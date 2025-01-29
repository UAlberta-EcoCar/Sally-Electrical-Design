# HyperDrive Motor Controller

## University of Alberta EcoCar Team; Abinash Singh

---

## Revision Notes

### Over all system notes:

- Remove the TMC4671 and just use the stm32g4. Implement SimpleFOC in HAL.
- Make a slow charge circuit for the power stage capacitors.

### Hardware Changes:

- Remove TMC4671 and make the board smaller.
- Use single mosfets instead of 2 in parrallel. This way the bridges can be smaller and fit in 100 by 100mm size constraint.
- Add a VM detect to the stm32, currently on the TMC4671 has detection for that.