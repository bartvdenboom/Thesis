###README
----------------
#####Instructions on how to run the Mealy machine inference process with StateLearner. The OpenThread commit used is [977cd00](https://github.com/openthread/openthread/tree/977cd004bf1736204910ab66559ce3871da95ba3)

* Follow the instructions and install [Statelearner](https://github.com/jderuiter/statelearner).
* Follow the instructions and build [OpenThread commit 977cd00](https://github.com/openthread/openthread/tree/977cd004bf1736204910ab66559ce3871da95ba3).
* Build OpenThread using the configuration from *configuration.txt*.
* Run mak-check to build the testing files inside *openthread-master/test/unit*.

----------------------
#####To build the SUT do the following:

* Replace contents of folder *openthread-master/scr/core* with contents of folder *cores*.
* Replace the contents of *openthread-master/tests/unit* with the contens of folder *openhtread/tests/unit* from this repository.
* In file *openthread/master/tests/unit/test_platform.cpp* choose the otPlatRadioGetIeeeEui64() of the SUT instance.
* Run make test-message

--------------------------
#####To build the mapper, do the following:

* Replace contents of folder *openthread-master/scr/core* with contents of folder *coret*.
* Replace the contents of *openthread-master/tests/unit* with the contens of folder *openhtread/tests/unit* from this repository if not already done.
* In file *openthread/master/tests/unit/test_platform.cpp* choose the otPlatRadioGetIeeeEui64() of the Mapper instnce.
* Run make test-fuzz

-----------------------
#####To start the inference process, do the folowwing:
* Run *./openthread-master/tests/unit/test-fuzz*
* Run the Statelearner socket example and specify the alpabet. Implemented messages can be found in *openthread-master/tests/unit/messages.cpp*.