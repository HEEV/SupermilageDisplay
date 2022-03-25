#pragma once

namespace spmg {
	
	/**
	* An object interface by which communications data
	* is exposed.
	*
	* @author Andrew Huffman
	* @version October 9, 2021
	*/
	class DataInterface {
		public:
		
		// CONSTRUCTORS
		
		/**
		* Default Constructor.
		*/
		DataInterface();
		
		
		// GETTERS
		
		/**
		* Gets the ground speed (speed of wheel spin)
		* in miles per hour.
		*
		* @return the ground speed.
		*/
		float getGroundSpeed() const;
		
		/**
		* Gets the wind speed in miles per hour.
		*
		* @return the wind speed.
		*/
		float getWindSpeed() const;
		
		/**
		* Gets the temperature of the engine in Fahrenheit.
		*
		* @return the temperature of the engine.
		*/
		float getEngineTemp() const;
		
		/**
		* Gets the elapsed time from the start of the race
		* in seconds.
		*
		* @return the elapsed time.
		*/
		float getElapsedTime() const;
		
		/**
		* Gets the number of laps completed thus far in the
		* race.
		*
		* @return the number of laps completed.
		*/
		unsigned int getLapCount() const;
		
		/**
		* Gets the air pressure in PSI.
		*
		* @return the air pressure.
		*/
		float getAirPressure() const;
		
		/**
		* Gets the fuel pressure in PSI.
		*
		* @return the fuel pressure.
		*/
		float getFuelPressure() const;
		
	};
	
}