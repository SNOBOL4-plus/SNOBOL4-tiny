package driver.jvm;

/**
 * bb_rpos.java — RPOS: assert cursor == Ω-n (zero-width)
 * Port of bb_rpos.c / bb_rpos.s
 *
 *   RPOS_α:  if (Δ != Ω-n)    goto RPOS_ω;
 *            RPOS=spec(Σ+Δ,0); goto RPOS_γ;
 *   RPOS_β:                    goto RPOS_ω;
 */
class bb_rpos extends bb_box {
    private final int n;

    public bb_rpos(MatchState ms, int n) { super(ms); this.n=n; }

    @Override public Spec alpha() {
        if (ms.delta != ms.omega - n) return null;
        return new Spec(ms.delta, 0);
    }

    @Override public Spec beta() { return null; }
}
