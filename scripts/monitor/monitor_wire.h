/*
 * monitor_wire.h — sync-step monitor binary wire format.
 *
 * Single source of truth for the binary record layout used by:
 *   - scripts/monitor/monitor_ipc_bin_csn.c   (CSNOBOL4 ABI)
 *   - x64/monitor_ipc_bin_spl.c               (SPITBOL ABI)
 *   - src/runtime/x86/snobol4.c mon_send      (scrip in-process)
 *   - scripts/monitor/monitor_sync.py         (controller)
 *
 * Wire format — fixed 13-byte header, optional value bytes:
 *
 *   offset  size  field        notes
 *   ------  ----  -----------  ----------------------------------------
 *      0     4    kind         u32 LE: 1=VALUE 2=CALL 3=RETURN 4=END 5=LABEL
 *      4     4    name_id      u32 LE: index into per-run names file
 *      8     1    type         u8: SNOBOL4 datatype, see MWT_*
 *      9     4    value_len    u32 LE: length of value_bytes that follow
 *     13   ...    value_bytes  raw bytes per type:
 *                                STRING / NAME    : raw bytes, length=value_len
 *                                INTEGER          : 8 bytes int64 LE, length=8
 *                                REAL             : 8 bytes IEEE754 double LE, length=8
 *                                NULL / PATTERN /
 *                                ARRAY / TABLE /
 *                                CODE / DATA /
 *                                EXPRESSION / FILE: empty, length=0
 *
 * Cross-dialect comparison is byte-for-byte equality on the full record.
 *
 * MWK_LABEL semantics: emitted by the runtime on every statement entry
 * (one per source statement, regardless of GOTO / fall-through).  The
 * name_id is MW_NAME_ID_NONE; the type is MWT_INTEGER and the 8-byte LE
 * payload carries the SNOBOL4 statement number (&STNO) of the statement
 * being entered.  Controllers align on STNO alignment without needing
 * a label-table reverse lookup; the source line / label name (if any)
 * can be derived externally from the program listing.
 *
 * The names file is a UTF-8 plain text sidecar: one name per line, indexed
 * 0..N-1.  inject_traces.py emits it; participants pass its path via
 * MONITOR_NAMES_FILE env var; controller reads it to print human-readable
 * traces.  name_id 0xffffffff is reserved for "(unnamed)" used by END events.
 */

#ifndef MONITOR_WIRE_H
#define MONITOR_WIRE_H

#include <stdint.h>

/* --- Event kinds (record.kind) -------------------------------------------- */
#define MWK_VALUE   1u
#define MWK_CALL    2u
#define MWK_RETURN  3u
#define MWK_END     4u
#define MWK_LABEL   5u   /* statement entry — carries STNO label name (or empty) */

/* --- SNOBOL4 datatype codes (record.type) --------------------------------- */
/* Chosen as a stable, dialect-neutral enumeration.  Both ABIs map their
 * internal type tags to these on emit; the controller never sees the
 * dialect-specific tags. */
#define MWT_NULL        0
#define MWT_STRING      1
#define MWT_INTEGER     2
#define MWT_REAL        3
#define MWT_NAME        4
#define MWT_PATTERN     5
#define MWT_EXPRESSION  6
#define MWT_ARRAY       7
#define MWT_TABLE       8
#define MWT_CODE        9
#define MWT_DATA       10
#define MWT_FILE       11
#define MWT_UNKNOWN   255   /* fallback for ABI-specific tags we don't decode */

/* --- Header on the wire is exactly 13 bytes ------------------------------- */
#define MW_HDR_BYTES   13

/* --- Reserved name_id for END events (no associated name) ----------------- */
#define MW_NAME_ID_NONE  0xffffffffu

/* --- Helpers to build a header in a stack buffer -------------------------- */
/* Caller provides a 13-byte buffer; this writes the header in LE byte order
 * regardless of host endianness.  Returns nothing — caller knows the size. */
static inline void mw_pack_hdr(unsigned char hdr[MW_HDR_BYTES],
                               uint32_t kind, uint32_t name_id,
                               uint8_t  type, uint32_t value_len)
{
    hdr[0]  = (unsigned char)( kind        & 0xff);
    hdr[1]  = (unsigned char)((kind  >>  8)& 0xff);
    hdr[2]  = (unsigned char)((kind  >> 16)& 0xff);
    hdr[3]  = (unsigned char)((kind  >> 24)& 0xff);
    hdr[4]  = (unsigned char)( name_id      & 0xff);
    hdr[5]  = (unsigned char)((name_id>>  8)& 0xff);
    hdr[6]  = (unsigned char)((name_id>> 16)& 0xff);
    hdr[7]  = (unsigned char)((name_id>> 24)& 0xff);
    hdr[8]  = type;
    hdr[9]  = (unsigned char)( value_len      & 0xff);
    hdr[10] = (unsigned char)((value_len>>  8)& 0xff);
    hdr[11] = (unsigned char)((value_len>> 16)& 0xff);
    hdr[12] = (unsigned char)((value_len>> 24)& 0xff);
}

#endif /* MONITOR_WIRE_H */
