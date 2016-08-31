#include <Arduino.h>
#include <Task.h>
#include <Wire.h>
#include <Sodaq_BMP085.h>

class TaskReadWeather : public Task
{
	public:
		typedef void(*action)(long a, float t);
		TaskReadWeather(action function, uint32_t timeInterval) : // pass any custom arguments you need
			Task(timeInterval),
			_callback(function)
		{ };

	private:
		const action _callback;
		Sodaq_BMP085 sensor;


		virtual bool OnStart() // optional
		{
			sensor.begin();
		}

		virtual void OnStop() // optional
		{
		}

		virtual void OnUpdate(uint32_t deltaTime)
		{
			float t = sensor.readTemperature();
			long a = sensor.readPressure();
			_callback(a, t);
		}
};
