# Embedded Pong
Recreation of the classic arcade game Pong using an STM32 Nucleo-64.

**Demo**
<table>
  <tr valign="bottom">
    <td align="center">
      <img src="media/PongDemo.mp4" alt="Results" width="60%">
    </td>
  </tr>
</table>

**Functionality:**

Embedded Pong contains all the core functionalities of the arcade game. The ball is served automtically, and the two players to rally until one scores on the other. The score 
is displayed via the seven segment display, and a winning message is displayed on the OLED once one player reaches 11 points. The game remembers the x and y axis velocity of
the ball, allowing dynamic velocity adjustments based on the movement of the paddle when it connects. A potentiometer allows players to adjust the speed of the ball.

**How It Works:**

The position of all game components on the OLED are updated routinely by a hardware timer. The game uses two coordinate systems: display coordinates to indicate where game 
objects appear on the 128x64 pixel OLED, and real coordinates which track the exact position of the ball, allowing smooth movements by maintaining the true location instead 
of snapping to the low resolution of the display each update. The game tracks the number of timer updates where the players have held down the paddle buttons, increasing
"excitement" variables that increase the ball velocity when it connects to the paddles. The potentiometer is read using an ADC and converted into a scalar that is multiplied 
with the ball velocity to increase game speed.

**Usage:**

**Licenses:**
