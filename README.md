# World
This project implements a simplified life simulator where virtual inhabitants are social and spend their lives moving around different locations, spreading information known as rumors.

## Introduction
A simulation is an experiment with a model that closely mimics reality and its evolution over time.
Discrete Event Simulation (DES) models the operation of a system as a sequence of discrete events in time. The model has a static structure and a dynamic structure:
- The static structure specifies the possible states of the model and is described as a collection of entities and their attributes. The state of the system is the collection of data that describes the system at a given moment.
- The dynamic structure specifies how the state changes over time and is usually defined in terms of events, which occur at a specific moment and change the state of the system.
Between consecutive events, the state of the system does not change, and the simulation can jump directly from the occurrence of one event to the next.

## Mundo ("World" in Portuguese)
The simulator maintain a global logical clock (an integer representing the amount of time in the real system), and an ordered list of future events (LEF - "Lista de Eventos Futuros", in Portuguese). The basic simulation cycle removes the first event from the event list, updates the state of the system, schedules new events in the list, and continues the cycle.

### Entities and Attributes
This section presents the entities and attributes of our virtual world.

#### World
Our world is defined by these entities and some general information.
- Current Time: A positive integer indicating the current time of the simulation. Each unit represents 15 minutes of real time;
- Rumors: A set of all rumors that can be known;
- NPeople: Total number of people in the world;
- NLocations: Total number of locations in the world;
- Max World Size: Maximum coordinates of the Cartesian plane of the locations;
- Time End of the World: A positive integer indicating the time for the end of the simulation.

#### Person
Represents the people in the simulation, and the attributes of the people determine their behavior in locations.
- Extroversion: An integer between 0 and 100 that indicates the person's level of extroversion. In our model, more extroverted people (higher numbers) are more likely to spread and hear rumors;
- Patience: An integer between 0 and 100 indicating how patient a person is. In our model, this affects decisions about staying in places and queues;
- Age: An integer between 18 and 100 indicating a person's age in years. In our simulation, age affects travel time between places;
- Set of Known Rumors (CRC - "Conjunto de Rumores Conhecidos", in Portuguese): Rumors known by a particular person that can be spread.

#### Location
Represents the places frequented by people.
- Maximum Capacity: The maximum number of people allowed in a place;
- People in Place: A set of identifiers of the people currently in the place;
- Queue: A queue where people wait to enter if it is full;
- Location: A pair of integers (x, y) indicating coordinates on a Cartesian plane. We will consider that the map of our world is represented by a Cartesian plane such that each unit represents 1 meter.

### Events

LibFila

LibConjunto

LibLEF

Conclusion
