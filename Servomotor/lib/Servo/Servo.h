#include <QuadratureEncoder.h>
#include <PID.h>
#include <HBridge.h>
#include <TF.h>

enum controlled_variable
{
    NONE,
    SPEED,
    POSITION,
    HAPTICM,
    HAPTICF
};

class Servo
{
public:
    QuadratureEncoder encoder;
    PID pid;
    HBridge driver;
    TF filter;
    void setControlMode(uint8_t mode);
    float calculateControlOutput(float ref, int dt_ms);

private:
    controlled_variable _control_mode = POSITION;
};
