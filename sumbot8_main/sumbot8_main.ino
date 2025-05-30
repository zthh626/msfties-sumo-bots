#include <XboxSeriesXControllerESP32_asukiaaa.hpp>
// Motor Controller Wiki: https://wiki.dfrobot.com/Dual_Channel_DC_Motor_Driver_12A_SKU_DFR0601

// Motor Controller 1 Signals
int PWM1_MC1 = 32;
int PWM2_MC1 = 19;
int INA1_MC1 = 33;
int INA2_MC1 = 18;
int INB1_MC1 = 25;
int INB2_MC1 = 5;

// Motor Controller 2 Signals
int PWM1_MC2 = 26;
int PWM2_MC2 = 17;
int INA1_MC2 = 27;
int INA2_MC2 = 16;
int INB1_MC2 = 14;
int INB2_MC2 = 4;

XboxSeriesXControllerESP32_asukiaaa::Core;
const char *XBOX_CONTROLLER_ADDRESS = "0c:35:26:e9:d8:e3";
xboxController(XBOX_CONTROLLER_ADDRESS);

void setupXboxController()
{
  Serial.begin(115200);
  Serial.println("Starting NimBLE Client");
  xboxController.begin();
}

void setupPins()
{
  // put your setup code here, to run once:
  // Motor 1
  pinMode(PWM1_MC1, OUTPUT);
  pinMode(INA1_MC1, OUTPUT);
  pinMode(INB1_MC1, OUTPUT);

  // Motor 2
  pinMode(PWM2_MC1, OUTPUT);
  pinMode(INA2_MC1, OUTPUT);
  pinMode(INB2_MC1, OUTPUT);

  // Motor 3
  pinMode(PWM1_MC2, OUTPUT);
  pinMode(INA1_MC2, OUTPUT);
  pinMode(INB1_MC2, OUTPUT);

  // Motor 4
  pinMode(PWM2_MC2, OUTPUT);
  pinMode(INA2_MC2, OUTPUT);
  pinMode(INB2_MC2, OUTPUT);
}

void setup()
{
  setupPins();
  setupXboxController();
}

void loop()
{
  xboxController.onLoop();
  void driveForwardMax();

  Serial.println("at " + String(millis()));
  delay(500);

  // goal is to poll for bluetooth signals to instruct robot to move
  /**sample instructions:
      RT button press+hold = drive forward and increase speed linearly to max
      B button press = turbo mode (10 secs)
      LT button press+hold = reverse and increase speed linearly to max
      RB button press + hold = turn robot sideways to right until button release
      LB button press + hold = turn robot sideways to left until button release
  **/

  // NOTE: when switching motor direction from forward to backward or vice versa, speed needs to be adjusted to 0 through PWM before switching
}

void pollXboxController()
{
  if (xboxController.isConnected())
  {
    if (xboxController.isWaitingForFirstNotification())
    {
      Serial.println("waiting for first notification");
    }
    else
    {
      Serial.println("Address: " + xboxController.buildDeviceAddressStr());
      Serial.print(xboxController.xboxNotif.toString());
      unsigned long receivedAt = xboxController.getReceiveNotificationAt();
      uint16_t joystickMax = XboxControllerNotificationParser::maxJoy;
      Serial.print("joyLHori rate: ");
      Serial.println((float)xboxController.xboxNotif.joyLHori / joystickMax);
      Serial.print("joyLVert rate: ");
      Serial.println((float)xboxController.xboxNotif.joyLVert / joystickMax);
      Serial.println("battery " + String(xboxController.battery) + "%");
      Serial.println("received at " + String(receivedAt));
    }
  }
  else
  {
    Serial.println("not connected");
    if (xboxController.getCountFailedConnection() > 2)
    {
      ESP.restart();
    }
  }
}

void driveForwardMax()
{
  // Initialize motor controller for forward motion
  digitalWrite(INA1_MC1, LOW);
  digitalWrite(INA2_MC1, LOW);
  digitalWrite(INA1_MC2, LOW);
  digitalWrite(INA2_MC2, LOW);

  digitalWrite(INB1_MC1, HIGH);
  digitalWrite(INB2_MC1, HIGH);
  digitalWrite(INB1_MC2, HIGH);
  digitalWrite(INB2_MC1, HIGH);

  // Set speed to max
  analogWrite(PWM1_MC1, 255);
  analogWrite(PWM2_MC1, 255);
  analogWrite(PWM1_MC2, 255);
  analogWrite(PWM2_MC2, 255);

  // Stop motors
  digitalWrite(INA1_MC1, LOW);
  digitalWrite(INA2_MC1, LOW);
  digitalWrite(INA1_MC2, LOW);
  digitalWrite(INA2_MC2, LOW);

  digitalWrite(INB1_MC1, LOW);
  digitalWrite(INB2_MC1, LOW);
  digitalWrite(INB1_MC2, LOW);
  digitalWrite(INB2_MC1, LOW);
}

// future functions
void rightTurn()
{
}

void leftTurn()
{
}
