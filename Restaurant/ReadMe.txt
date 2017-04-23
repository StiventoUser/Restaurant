========================================================================
    CONSOLE APPLICATION : Restaurant Project Overview
========================================================================

**RestaurantCore: main class
open(): opens the resturant and begins the day
close(): close the restaurant after the end of a day. Also is called from destructor

**ThreadWrapper: controls an object lifetime 
getObject(): returns pointer to the controlled object
isFinished(): checks has the object finished its work
execute(): begins a new thread for the object

**Cook: represents a cook
work(): simulates the work (executed in another thread)

**Waiter: represents a waiter
work(): simulates the work (executed in another thread)

**RestaurantInternal: encapsulates inter-thread communication and restaurant's internal data
requestedDishName [get/set]: the dish name that is sent to the cook
dishInfo [get/set]: prepared dish that is delivered by waiter
restaurantClosed [get/set]: specifies that the restaurant should be closed and all threads must be terminated
lockData(): ensures that the data can be modified
notify(): generates an event
waitFor(): blocks current thread until the specific event

**Dish: represents a prepared dish

**Logger: a thread-safe class to output data with displaying time offset
