# STM32G431_FOC_SingleSystem
  This is a repository for FOC SingleSystem,which is based on STM32G431 platform,including OpenLoop Uq Control Mode,CloseLoop Current Control Mode,CloseLoop Speed Control Mode and CloseLoop Angle Control Mode in indenpendent project,then using wireless module on the base of UART Communication to control motor of different states on the PC.

# Contents
* [Framework](#Framework)
* [Control Strategy](#Control-Strategy)
* [Development Log](#Development-Log)

---
## Framework
| Content | Use |
| :---: | :---: |
| Motor Control | FOC+SVPWM with PID |
| Current Sample | Dual resistor Low-side |
| Angle Sensor | AS5600 Base on I2C |
| Motor | PMSM with 7 pole pairs |

---
## Control Strategy
* [OpenLoop Uq Control](#OpenLoop-Uq-Control)
* [CloseLoop Current Control](#CloseLoop-Current-Control)
* [CloseLoop Speed Control](#CloseLoop-Speed-Control)
* [CloseLoop Angle Control](#CloseLoop-Angle-Control)

  All of the above is using the Control Strategy of Zero Angle aligning on the Park-transform (d-axis set constant while q-axis setting zero) then only setting q-axis constant to start pulling the motor,at that time the whole system will operate by Rev-Park and SVPWM module and generate the three-phase sinewave to control the motor rolling with read-time parameters.

### OpenLoop Uq Control
  Base on the pulling,only setting the Uq constant then updating electrical angle in real-time on the Park-axiy.

### CloseLoop Current Control
![I_pid](https://github.com/user-attachments/assets/967786aa-eb30-4e74-a044-c6ac7aeb0e3b)
  Current Sampling module begin to work with Clark-transform and Park-transform to get the actual Iq and Id,then inputting to Current-PID Loop to achieve CloseLoop Iq and Id control.  

### CloseLoop Speed Control
![Speed_I_pid](https://github.com/user-attachments/assets/46d9e57c-9a36-40f4-bee1-278fcb9aebf8)
  Base on the Current-PID Loop,Calculating actual motor speed and inputting to Speed-PID Loop in the outer loop of the Current-PID Loop to achieve CloseLoop Speed control.

### CloseLoop Angle Control
![Position_Speed_I_pid](https://github.com/user-attachments/assets/b225fc17-abc0-4670-b00d-df90691b22cb)
  Base on the Speed-PID Loop,Calculating actual motor Angle and inputting to Angle-PID Loop in the outer loop of the Speed-PID Loop to achieve CloseLoop Angle control.  

---
## Development Log
🚀 [v1.0.0] - 2026-01-21
* added
    * Achieve control motor by 4 modes in each indenpendent project on the PC.    
