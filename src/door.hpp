class Button
{
public:
	void init(int pin)
	{
		this->pin = pin;
		pinMode(pin, INPUT);
	}
	
	bool isActive(int read_count = 3)
	{
		/* not for a touch screen button */
		bool value = false;
		for(int i = 0; i < read_count; ++i)
		{
			if(digitalRead(pin) == HIGH)
			{
				value = true;
				continue;
			}
			value = false;
			break;
		}
		while(value)
		{
			if(digitalRead(pin) == LOW)
			{
				break;
			}
		}
		
		return value;
	}
	
protected:
	int pin;
};



/* in the future Button & Sensor might become different */
class Sensor
{
public:
	void init(int pin)
	{
		this->pin = pin;
		pinMode(pin, INPUT);
	}
	
	bool isActive(int read_count = 2)
	{
		bool value = false;
		if(digitalRead(pin) == HIGH)
		{
			value = true;
		}
		return value;
	}
	
protected:
	int pin;
};



class Motor
{
public:
	enum State
	{
		STOP, FORWARD, BACKWARD
	};
	
	void init(int pin_A, int pin_B, bool inverted = false)
	{
		Serial.println("motor init");
				
		this->pin_A = pin_A;
		this->pin_B = pin_B;
		this->inverted = inverted;
		curr_state = STOP;

		pinMode(pin_A, OUTPUT);
		pinMode(pin_B, OUTPUT);
	}

	void forward(int speed = 0)
	{
		if(curr_state != FORWARD || this->speed != speed)
		{
			digitalWrite(pin_A, (!inverted) ? HIGH : LOW );
			digitalWrite(pin_B, (!inverted) ? LOW  : HIGH);

			curr_state = FORWARD;
			this->speed = speed;

			Serial.println("Motor forward");
		}
	}
	
	void backward(int speed = 0)
	{
		if(curr_state != BACKWARD || this->speed != speed)
		{
			digitalWrite(pin_A, (!inverted) ? LOW  : HIGH);
			digitalWrite(pin_B, (!inverted) ? HIGH : LOW );

			curr_state  = BACKWARD;
			this->speed = speed;

			Serial.println("Motor backward");
		}
	}

	void stop()
	{
		if(curr_state != STOP)
		{
			digitalWrite(pin_A, LOW);
			digitalWrite(pin_B, LOW);

			curr_state = STOP;
			this->speed = 0;
			
			Serial.println("Motor stoped");
		}
	}

protected:
	int pin_A = 0, pin_B = 0;
	State curr_state = STOP;
	int speed = 0;
	bool inverted = false;
};



class Door
{
	enum State
	{
		Closed, Opened, Closing, Opening, Interrupted
	};
	
public:
	void init()
	{
		/* read current state of the door and do something */
		
	}

	void loop()
	{
		switch(getCurrState())
		{
		case Closed:
			Serial.println("Closed");
			
			if(button_door->isActive())
			{
				setState(Opening);
			}
		
			closed();
		
			break;

		
		case Opening:
			Serial.println("Opening");
			
			if(sensor_open->isActive())
			{
				setState(Opened);
			}
			else if(button_door->isActive())
			{
				setState(Interrupted);
			}
		
			opening();
		
			break;


		case Opened:
			Serial.println("Opened");

			if(button_door->isActive())
			{
				setState(Closing);
			}

			opened();

			break;


		case Closing:
			Serial.println("Closing");

			if(sensor_close->isActive())
			{
				setState(Closed);
			}
			else if(button_door->isActive())
			{
				setState(Interrupted);
			}

			closing();

			break;
		

		case Interrupted:
			Serial.println("Inrerrupted");

			if(button_door->isActive())
			{
				switch(getPrevState())
				{
				case Closing:
					setState(Opening);
					break;
				
				case Opening:
					setState(Closing);
					break;	
				}
			}

			stop();

			break;
		}
	}

	void bindDoorButton(Button *button)
	{
		this->button_door = button;
	}

	void bindCloseSensor(Sensor *sensor)
	{
		this->sensor_open = sensor;
	}

	void bindOpenSensor(Sensor *sensor)
	{
		this->sensor_close = sensor;
	}

	void bindMotorA(Motor *motor)
	{
		this->motor_A = motor;
	}

	void bindMotorB(Motor *motor)
	{
		this->motor_B = motor;
	}
	
	void setState(State state)
	{
		prev_state = curr_state;
		curr_state = state;
	}
	
	State getCurrState()
	{
		return curr_state;
	}
	
	State getPrevState()
	{
		return prev_state;
	}

	void opening()
	{
		motor_A->forward();
		motor_B->backward();
	}

	void closing()
	{
		motor_A->backward();
		motor_B->forward();
	}

	void opened()
	{
		/* keep the door opened */
		motor_A->stop();
		motor_B->stop();
	}

	void closed()
	{
		motor_A->stop();
		motor_B->stop();
		/* keep the door closed */
	}

	void stop()
	{
		motor_A->stop();
		motor_B->stop();
	}
	
protected:
	State   curr_state = Closed, prev_state = Closed;
	Button *button_door = NULL;
	Sensor *sensor_open = NULL, *sensor_close = NULL;
	Motor  *motor_A, *motor_B;
};
