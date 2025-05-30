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

const char *XBOX_CONTROLLER_ADDRESS = "0c:35:26:e9:d8:e3";

XboxSeriesXControllerESP32_asukiaaa::Core
    xboxController(XBOX_CONTROLLER_ADDRESS);

struct XboxControllerState
{
  float joyLVertRate;
  float joyLHoriRate;
  bool bButtonPressed;
};

XboxControllerState pollXboxController();
void setupXboxController();
void setupPins();
void setup();
void loop();
void driveForwardMax();
void driveBackwardMax();
void stopMotors();
void rightTurn();
void leftTurn();

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
  // Poll the Xbox controller for the current state
  XboxControllerState state = pollXboxController();

  // Define tolerance for joystick center position
  const float tolerance = 0.02; // 2% tolerance

  // Joystick vertical values:
  // - Center: 0.50 (with tolerance range for stopping)
  // - Top: 0
  // - Bottom: 1
  if (state.joyLVertRate < (0.50 - tolerance))
  {
    driveForwardMax();
  }
  else if (state.joyLVertRate > (0.50 + tolerance))
  {
    driveBackwardMax();
  }
  else
  {
    stopMotors();
  }

  // Add a small delay to prevent excessive polling
  delay(1000);
}

XboxControllerState pollXboxController()
{
  XboxControllerState state = {0.0, 0.0, false};

  xboxController.onLoop();
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
      state.joyLVertRate = (float)xboxController.xboxNotif.joyLVert / joystickMax;
      state.joyLHoriRate = (float)xboxController.xboxNotif.joyLHori / joystickMax;
      state.bButtonPressed = xboxController.xboxNotif.btnB;
      Serial.print("joyLVert rate: ");
      Serial.println(state.joyLVertRate);
      Serial.print("joyLHori rate: ");
      Serial.println(state.joyLHoriRate);
      Serial.println("B button pressed: " + String(state.bButtonPressed));
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

  return state;
}

void driveForwardMax()
{
  // Set motors for forward motion
  digitalWrite(INA1_MC1, LOW);
  digitalWrite(INA2_MC1, LOW);
  digitalWrite(INA1_MC2, LOW);
  digitalWrite(INA2_MC2, LOW);

  digitalWrite(INB1_MC1, HIGH);
  digitalWrite(INB2_MC1, HIGH);
  digitalWrite(INB1_MC2, HIGH);
  digitalWrite(INB2_MC2, HIGH);

  // Set speed to max
  analogWrite(PWM1_MC1, 255);
  analogWrite(PWM2_MC1, 255);
  analogWrite(PWM1_MC2, 255);
  analogWrite(PWM2_MC2, 255);
}

void driveBackwardMax()
{
  // Set motors for backward motion
  digitalWrite(INA1_MC1, HIGH);
  digitalWrite(INA2_MC1, HIGH);
  digitalWrite(INA1_MC2, HIGH);
  digitalWrite(INA2_MC2, HIGH);

  digitalWrite(INB1_MC1, LOW);
  digitalWrite(INB2_MC1, LOW);
  digitalWrite(INB1_MC2, LOW);
  digitalWrite(INB2_MC2, LOW);

  // Set speed to max
  analogWrite(PWM1_MC1, 255);
  analogWrite(PWM2_MC1, 255);
  analogWrite(PWM1_MC2, 255);
  analogWrite(PWM2_MC2, 255);
}

void stopMotors()
{
  // Stop all motors
  digitalWrite(INA1_MC1, LOW);
  digitalWrite(INA2_MC1, LOW);
  digitalWrite(INA1_MC2, LOW);
  digitalWrite(INA2_MC2, LOW);

  digitalWrite(INB1_MC1, LOW);
  digitalWrite(INB2_MC1, LOW);
  digitalWrite(INB1_MC2, LOW);
  digitalWrite(INB2_MC2, LOW);

  // Set speed to zero
  analogWrite(PWM1_MC1, 0);
  analogWrite(PWM2_MC1, 0);
  analogWrite(PWM1_MC2, 0);
  analogWrite(PWM2_MC2, 0);
}

// future functions
void rightTurn()
{
}

void leftTurn()
{
}
