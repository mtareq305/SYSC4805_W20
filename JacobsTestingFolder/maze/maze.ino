#define leftFarSensor 13

#define leftNearSensor 12

#define CenterSensor 11

#define rightNearSensor 7

#define rightFarSensor 6

int leftNearReading;

int leftFarReading;

int CenterReading;

int rightNearReading;

int rightFarReading;

int leftNudge;

int replaystage;

int rightNudge;

#define leapTime 200

#define leftMotor1 2

#define leftMotor2 3

#define rightMotor1 4

#define rightMotor2 5

#define en1 9

#define en2 10

char path[30] = {};

int pathLength;

int readLength;

void setup()

{

    pinMode(leftNearSensor, INPUT);

    pinMode(leftFarSensor, INPUT);

    pinMode(CenterSensor, INPUT);

    pinMode(rightNearSensor, INPUT);

    pinMode(rightFarSensor, INPUT);

    pinMode(leftMotor1, OUTPUT);

    pinMode(leftMotor2, OUTPUT);

    pinMode(rightMotor1, OUTPUT);

    pinMode(rightMotor2, OUTPUT);

    pinMode(en1, OUTPUT);

    pinMode(en2, OUTPUT);

    analogWrite(en1, 160);

    analogWrite(en2, 160);

    //Serial.begin(115200);

    delay(1000);
}

void loop()

{

    readSensors();

    if (leftFarReading == LOW && rightFarReading == LOW &&

        (CenterReading == HIGH || leftNearReading == HIGH))

    {

        straight();
    }

    else

    {

        leftHandWall();
    }
}

void readSensors()

{

    leftNearReading = digitalRead(leftNearSensor);

    leftFarReading = digitalRead(leftFarSensor);

    CenterReading = digitalRead(CenterSensor);

    rightNearReading = digitalRead(rightNearSensor);

    rightFarReading = digitalRead(rightFarSensor);

    // serial printing below for debugging purposes

    // Serial.print("leftCenterReading: ");

    // Serial.println(leftCenterReading);

    // Serial.print("leftNearReading: ");

    // Serial.println(leftNearReading);

    // Serial.print("leftFarReading: ");

    // Serial.println(leftFarReading);

    // Serial.print("rightCenterReading: ");

    // Serial.println(rightCenterReading);

    // Serial.print("rightNearReading: ");

    // Serial.println(rightNearReading);

    // Serial.print("rightFarReading: ");

    // Serial.println(rightFarReading);

    // delay(200);
}

void leftHandWall()

{

    if (leftFarReading == HIGH && rightFarReading == HIGH)
    {

        digitalWrite(leftMotor1, HIGH);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, HIGH);

        digitalWrite(rightMotor2, LOW);

        delay(300);

        readSensors();

        if (leftFarReading == LOW && rightFarReading == LOW)

        {

            turnLeft();
        }

        else

        {

            done();
        }
    }

    if (leftFarReading == HIGH)

    {

        digitalWrite(leftMotor1, HIGH);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, HIGH);

        digitalWrite(rightMotor2, LOW);

        delay(300);

        readSensors();

        if (leftFarReading == LOW && rightFarReading == LOW)

        {

            turnLeft();
        }

        else

        {

            done();
        }
    }

    if (rightFarReading == HIGH)

    {

        digitalWrite(leftMotor1, HIGH);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, HIGH);

        digitalWrite(rightMotor2, LOW);

        delay(300);

        readSensors();

        if (leftFarReading == HIGH)

        {

            delay(leapTime - 30);

            readSensors();

            if (rightFarReading == HIGH && leftFarReading == HIGH)

            {

                done();
            }

            else if (rightFarReading == LOW && leftFarReading == LOW && CenterReading == LOW && rightNearReading == LOW)

            {

                turnRight();
            }

            else

            {

                straight();

                return;
            }
        }

        delay(leapTime - 30);

        readSensors();

        if (leftFarReading == LOW && CenterReading == LOW &&

            rightFarReading == LOW)
        {

            turnRight();

            return;
        }

        path[pathLength] = 'S';

        // Serial.println("s");

        pathLength++;

        //Serial.print("Path length: ");

        //Serial.println(pathLength);

        if (path[pathLength - 2] == 'B')

        {

            //Serial.println("shortening path");

            shortPath();
        }

        straight();
    }

    readSensors();

    if (leftFarReading == LOW && CenterReading == LOW

        && rightFarReading == LOW && leftNearReading == LOW && rightNearReading == LOW)

    {

        turnAround();
    }
}

void done()

{

    digitalWrite(leftMotor1, LOW);

    digitalWrite(leftMotor2, LOW);

    digitalWrite(rightMotor1, LOW);

    digitalWrite(rightMotor2, LOW);

    replaystage = 1;

    path[pathLength] = 'D';

    pathLength++;

    delay(1500);

    replay();
}

void turnLeft()

{

    while (digitalRead(leftNearSensor) == HIGH)

    {

        digitalWrite(leftMotor1, LOW);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, HIGH);

        digitalWrite(rightMotor2, LOW);

        delay(2);

        digitalWrite(leftMotor1, LOW);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, LOW);

        digitalWrite(rightMotor2, LOW);

        delay(1);
    }

    while (digitalRead(leftNearSensor) == LOW)

    {

        digitalWrite(leftMotor1, LOW);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, HIGH);

        digitalWrite(rightMotor2, LOW);

        delay(2);

        digitalWrite(leftMotor1, LOW);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, LOW);

        digitalWrite(rightMotor2, LOW);

        delay(1);
    }

    while (digitalRead(leftFarSensor) == HIGH && digitalRead(leftNearSensor) == HIGH)

    {

        digitalWrite(leftMotor1, LOW);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, HIGH);

        digitalWrite(rightMotor2, LOW);

        delay(2);

        digitalWrite(leftMotor1, LOW);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, LOW);

        digitalWrite(rightMotor2, LOW);

        delay(1);
    }

    if (replaystage == 0)
    {

        path[pathLength] = 'L';

        //Serial.println("l");

        pathLength++;

        //Serial.print("Path length: ");

        //Serial.println(pathLength);

        if (path[pathLength - 2] == 'B')
        {

            //Serial.println("shortening path");

            shortPath();
        }
    }
}

void turnRight()

{

    while (digitalRead(rightNearSensor) == HIGH)
    {

        digitalWrite(leftMotor1, HIGH);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, LOW);

        digitalWrite(rightMotor2, LOW);

        delay(1);

        digitalWrite(leftMotor1, LOW);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, LOW);

        digitalWrite(rightMotor2, LOW);

        delay(1);
    }

    while (digitalRead(rightNearSensor) == LOW)
    {

        digitalWrite(leftMotor1, HIGH);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, LOW);

        digitalWrite(rightMotor2, LOW);

        delay(1);

        digitalWrite(leftMotor1, LOW);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, LOW);

        digitalWrite(rightMotor2, LOW);

        delay(1);
    }

    while (digitalRead(leftNearSensor) == LOW)
    {

        digitalWrite(leftMotor1, HIGH);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, LOW);

        digitalWrite(rightMotor2, LOW);

        delay(1);

        digitalWrite(leftMotor1, LOW);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, LOW);

        digitalWrite(rightMotor2, LOW);

        delay(1);
    }

    if (replaystage == 0)
    {

        path[pathLength] = 'R';

        Serial.println("r");

        pathLength++;

        Serial.print("Path length: ");

        Serial.println(pathLength);

        if (path[pathLength - 2] == 'B')
        {

            Serial.println("shortening path");

            shortPath();
        }
    }
}

void straight()
{

    if (digitalRead(leftNearSensor) == LOW)
    {

        digitalWrite(leftMotor1, HIGH);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, HIGH);

        digitalWrite(rightMotor2, LOW);

        delay(1);

        digitalWrite(leftMotor1, HIGH);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, LOW);

        digitalWrite(rightMotor2, LOW);

        delay(1);

        return;
    }

    if (digitalRead(rightNearSensor) == LOW)
    {

        digitalWrite(leftMotor1, HIGH);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, HIGH);

        digitalWrite(rightMotor2, LOW);

        delay(1);

        digitalWrite(leftMotor1, LOW);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, HIGH);

        digitalWrite(rightMotor2, LOW);

        delay(1);

        return;
    }

    digitalWrite(leftMotor1, HIGH);

    digitalWrite(leftMotor2, LOW);

    digitalWrite(rightMotor1, HIGH);

    digitalWrite(rightMotor2, LOW);

    delay(1);

    digitalWrite(leftMotor1, LOW);

    digitalWrite(leftMotor2, LOW);

    digitalWrite(rightMotor1, LOW);

    digitalWrite(rightMotor2, LOW);

    delay(1);
}

void turnAround()
{

    digitalWrite(leftMotor1, HIGH);

    digitalWrite(leftMotor2, LOW);

    digitalWrite(rightMotor1, HIGH);

    digitalWrite(rightMotor2, LOW);

    delay(100);

    while (digitalRead(leftNearSensor) == LOW || digitalRead(CenterSensor) == LOW)
    {

        digitalWrite(leftMotor1, LOW);

        digitalWrite(leftMotor2, HIGH);

        digitalWrite(rightMotor1, HIGH);

        digitalWrite(rightMotor2, LOW);

        delay(1);

        digitalWrite(leftMotor1, LOW);

        digitalWrite(leftMotor2, LOW);

        digitalWrite(rightMotor1, LOW);

        digitalWrite(rightMotor2, LOW);

        delay(1);
    }

    path[pathLength] = 'B';

    pathLength++;

    straight();

    //Serial.println("b");

    //Serial.print("Path length: ");

    //Serial.println(pathLength);
}

void shortPath()
{

    int shortDone = 0;

    if (path[pathLength - 3] == 'L' && path[pathLength - 1] == 'R')
    {

        pathLength -= 3;

        path[pathLength] = 'B';

        //Serial.println("test1");

        shortDone = 1;
    }

    if (path[pathLength - 3] == 'L' && path[pathLength - 1] == 'S' && shortDone == 0)
    {

        pathLength -= 3;

        path[pathLength] = 'R';

        //Serial.println("test2");

        shortDone = 1;
    }

    if (path[pathLength - 3] == 'R' && path[pathLength - 1] == 'L' && shortDone == 0)
    {

        pathLength -= 3;

        path[pathLength] = 'B';

        //Serial.println("test3");

        shortDone = 1;
    }

    if (path[pathLength - 3] == 'S' && path[pathLength - 1] == 'L' && shortDone == 0)
    {

        pathLength -= 3;

        path[pathLength] = 'R';

        //Serial.println("test4");

        shortDone = 1;
    }

    if (path[pathLength - 3] == 'S' && path[pathLength - 1] == 'S' && shortDone == 0)
    {

        pathLength -= 3;

        path[pathLength] = 'B';

        //Serial.println("test5");

        shortDone = 1;
    }

    if (path[pathLength - 3] == 'L' && path[pathLength - 1] == 'L' && shortDone == 0)
    {

        pathLength -= 3;

        path[pathLength] = 'S';

        //Serial.println("test6");

        shortDone = 1;
    }

    path[pathLength + 1] = 'D';

    path[pathLength + 2] = 'D';

    pathLength++;

    //Serial.print("Path length: ");

    //Serial.println(pathLength);

    //printPath();
}

void printPath()
{

    Serial.println("----------------");

    int x;

    while (x <= pathLength)
    {

        Serial.println(path[x]);

        x++;
    }

    Serial.println("-----------------");
}

void replay()
{

    readSensors();

    if (leftFarReading == LOW && rightFarReading == LOW)
    {

        straight();
    }

    else
    {

        if (path[readLength] == 'D')

        {

            digitalWrite(leftMotor1, HIGH);

            digitalWrite(leftMotor2, LOW);

            digitalWrite(rightMotor1, HIGH);

            digitalWrite(rightMotor2, LOW);

            delay(100);

            digitalWrite(leftMotor1, LOW);

            digitalWrite(leftMotor2, LOW);

            digitalWrite(rightMotor1, LOW);

            digitalWrite(rightMotor2, LOW);

            endMotion();
        }

        if (path[readLength] == 'L')

        {

            digitalWrite(leftMotor1, HIGH);

            digitalWrite(leftMotor2, LOW);

            digitalWrite(rightMotor1, HIGH);

            digitalWrite(rightMotor2, LOW);

            delay(300);

            turnRight();
        }

        if (path[readLength] == 'L')
        {

            digitalWrite(leftMotor1, HIGH);

            digitalWrite(leftMotor2, LOW);

            digitalWrite(rightMotor1, HIGH);

            digitalWrite(rightMotor2, LOW);

            delay(300);

            turnLeft();
        }

        if (path[readLength] == 'S')
        {

            digitalWrite(leftMotor1, HIGH);

            digitalWrite(leftMotor2, LOW);

            digitalWrite(rightMotor1, HIGH);

            digitalWrite(rightMotor2, LOW);

            delay(300);

            straight();
        }

        readLength++;
    }

    replay();
}

void endMotion()

{

    endMotion();
}