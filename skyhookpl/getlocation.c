#include "./wpsapi.h"
#include <stdio.h>
#include <stdlib.h>
double *getLocation()
{
	// initialize the WPS API
	WPS_load();

	// set the API key
	//(found in my.skyhook.com under projects -> the project you're developing)
	const char *key = "YOUR_KEY_HERE";
	WPS_set_key(key);

	// get the location
	WPS_Location *location;
	WPS_ReturnCode rc;

	rc = WPS_location(NULL, WPS_NO_STREET_ADDRESS_LOOKUP, &location);
	if (rc != WPS_OK)
	{
		fprintf(stderr, "*** WPS_location failed (%d)!\n", rc);
	}
	else
	{
		double *coordinates = malloc(sizeof(double) * 2);
		coordinates[0] = location->longitude;
		coordinates[1] = location->latitude;

		// free resources being used by the WPS_location object
		WPS_free_location(location);

		// free all resources being used by the WPS API
		WPS_unload();

		/*
		   * Units of coordinates are either in DD (Decimal Degrees) (or) in DMS (Degrees, Minutes and Seconds)
		   * Latitude - North or South of the Equator. If North, then the DD value is positive, otherwise negative.
		   * Longitude - East or West of the Prime Meridian. If East, then the DD value is positive, otherwise negative.
		*/

		// return the coordinates to the calling function
		return coordinates;
	}
}
