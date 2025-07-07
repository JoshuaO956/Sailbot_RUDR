/**************************************************************************
 * RudderControlLibrary – function skeletons with step-by-step intentions *
 * (Absolute-encoder version – code removed, logic preserved as comments) *
 **************************************************************************/

/*---------------------------------------------------------
  Helper: DAC_STEP()  –  manual output for bench experiments
-----------------------------------------------------------*/
void DAC_STEP(int step /*0-50*/) {

    // 1. Validate and clamp ‘step’ to the 0…50 range.
    // 2. Convert that step (in 2 % increments) to a 12-bit DAC value.
    // 3. Write that raw value to DAC channel 2 (right-aligned).
    // 4. Compute the equivalent output voltage (VREF * step / 50).
    // 5. Print both the raw DAC code and the voltage for the lab log.

    return;
}


/*---------------------------------------------------------
  Helper: Set_Motor()  –  low-level speed + direction driver
-----------------------------------------------------------*/
void Set_Motor(float power /*−1.0 … +1.0*/) {

    // 1. Take the absolute value of ‘power’ and scale it to 0…4095.
    // 2. Write that value to the DAC to command motor speed.
    // 3. Use the sign of ‘power’ to set GPIO PF13:
    //      * HIGH  =  forward rudder motion
    //      * LOW   =  reverse rudder motion
    // 4. Return to caller.

    return;
}


/*---------------------------------------------------------
  Main loop: PI_Motor() – 50 Hz closed-loop rudder controller
-----------------------------------------------------------*/
void PI_Motor(int32_t  desiredAngle,    // target from navigation logic
              int32_t  currentAngle,    // absolute encoder reading
              float    *integralError,  // accumulator (passed by ref)
              uint32_t *lastTimeStamp,  // ms tick from previous call
              int8_t  *prevAngle)      // last encoder angle (for velocity)
{
    /* 1. Time update -------------------------------------------------- */

    // * Read current MCU tick in ms.
    // * Compute del_t = (now − lastTimeStamp)/1000  → seconds.
    // * If del_t < 1 ms, force del_t = 1 ms to avoid divide-by-zero.

    /* 2. Command safety clamp ---------------------------------------- */

    // * Limit ‘desiredAngle’ to ±MAX_ANGLE to protect linkage.

    /* 3️. Error + direction ------------------------------------------- */

    // * error   = desiredAngle − currentAngle.
    // * dirCmd  = sign(error)  (−1, 0, +1).

    /* 4️. Dead-band check --------------------------------------------- */

    // * If |error| < ERROR_THRESHOLD:
    //     – call Set_Motor(0) to stop.
    //     – zero integralError.
    //     – store now in lastTimeStamp.
    //     – store currentAngle in prevAngle.
    //     – return;   (rudder is “good enough”)

    /* 5️. Shaft velocity measurement ---------------------------------- */
    // * Compute angular velocity = (currentAngle − prevAngle) / del_t.
    // * If |velocity| < VEL_EPS -> treat as 0 (noise filter).

    /* 6️. Safe-reverse guard ------------------------------------------ */
    // * If velocity is non-zero AND sign(velocity) ≠ dirCmd:
    //     – call Set_Motor(0) to coast.
    //     – update lastTimeStamp and prevAngle.
    //     – return;   (wait until shaft stops before reversing)

    /* 7️. PI control computation -------------------------------------- */
    // * integralError += error × del_t.
    // * Clamp integralError to ±INTEGRAL_LIMIT (anti-windup).
    // * ctrl = KP×error + KI×integralError.
    // * Clamp ctrl to ±MAX_MOTOR.
    // * If |ctrl| < MIN_MOTOR -> set to dirCmd × MIN_MOTOR
    //   (beats static friction / dead-zone).

    /* 8️. Actuation ---------------------------------------------------- */
    // * Call Set_Motor(ctrl) to send speed & direction to hardware.

    /* 9️. Update variable for next function call ------------------------------------------------- */
    // * Save ‘now’   → *lastTimeStamp.
    // * Save currentAngle → *prevAngle.
    // * Return to caller (loop will run again in 20 ms).

    return;
}

/* Notes:
   * Absolute encoder ensures currentAngle is always true; no wrap
     handling needed for ±45° steering span.
   * KP, KI, thresholds, and limits are tuned offline on a 3D printed jig,
     then refined during sea trials.
 */