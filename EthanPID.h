#pragma systemFile
//New PID with Struct!
typedef struct
{
	float m_p;   // P coefficient
	float m_i;   // I coefficient

	int m_desiredValue; // Desired value
	int m_errorSum; // Sum of previous errors (for I calculation)
	int m_errorIncrement; // Max increment to error sum each call

	bool m_firstCycle; // Flag for first cycle
	int m_maxOutput; // Ceiling on calculation output
}PIDStruct;

void EthanPID(PIDStruct &pid)
{
	pid.m_desiredValue = 0; // Default to 0, set later by the user
	pid.m_firstCycle = true;
	pid.m_maxOutput = 100; // Default to full range
	pid.m_errorIncrement = 1;
}

void setConstants(PIDStruct &pid, float p, float i)
{
	pid.m_p = p;
	pid.m_i = i;
}

void setErrorIncrement(PIDStruct &pid, float inc)
{
	pid.m_errorIncrement = inc;
}

void setDesiredValue(PIDStruct &pid, int val)
{
	pid.m_desiredValue = val;
}

void setMaxOutput(PIDStruct &pid, int max)
{
	if(max >= 0.0 && max <= 100)
	{
		pid.m_maxOutput = max;
	}
}

float calcPID(PIDStruct &pid, float currentValue)
{
	// Initialize all components to 0.0 to start.
	float pVal = 0.0;
	float iVal = 0.0;

	// Don't apply D the first time through.
	if(pid.m_firstCycle)
	{
		pid.m_firstCycle = false;
	}

	// Calculate P Component.
	float error = pid.m_desiredValue - currentValue;
	pVal = pid.m_p * (float)error;

	// Calculate I Component.
	// Error is positive and outside the epsilon band.
	if(error >= 0)
	{
		// Check if epsilon was pushing in the wrong direction.
		if(pid.m_errorSum < 0)
		{
			// If we are fighting away from the point, reset the error.
			pid.m_errorSum = 0;
		}
		if(error < pid.m_errorIncrement)
		{
			// If the error is smaller than the max increment amount, add it.
			pid.m_errorSum += error;
		}
		else
		{
			// Otherwise, add the maximum increment per cycle.
			pid.m_errorSum += pid.m_errorIncrement;
		}
	}
	// Error is negative and outside the epsilon band.
	else if(error <= -0)
	{
		if(pid.m_errorSum > 0)
		{
			// If we are fighting away from the point, reset the error.
			pid.m_errorSum = 0;
		}
		// error is small than max contribution -> just subtract error amount
		if(error > -pid.m_errorIncrement)
		{
			// If the error is smaller than the max increment amount, add it.
			pid.m_errorSum += error; // Error is negative
		}
		else
		{
			// Otherwise, subtract the maximum increment per cycle.
			pid.m_errorSum -= pid.m_errorIncrement;
		}
	}
	// Error is inside the epsilon band.
	else
	{
		pid.m_errorSum = 0;
	}
	iVal = pid.m_i * (float)pid.m_errorSum;

	// Calculate and limit the ouput: Output = P + I
	float output = pVal + iVal;
	if(output > pid.m_maxOutput)
	{
		output = pid.m_maxOutput;
	}
	else if(output < -pid.m_maxOutput)
	{
		output = -pid.m_maxOutput;
	}
	return output;
}
