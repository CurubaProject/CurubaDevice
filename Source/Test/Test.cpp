#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

#include "testCommsManager.h"

void runAllTests(int argc, char const *argv[]){
	cute::suite s;

	// Add your test here
	s.push_back(CUTE(test_pushTransmit));
	s.push_back(CUTE(test_MultiplePushTransmit));

	s.push_back(CUTE(test_receiveComms));
	s.push_back(CUTE(test_MultipleReceiveComms));

	cute::xml_file_opener xmlfile(argc,argv);
	cute::xml_listener<cute::ide_listener<> >  lis(xmlfile.out);
	cute::makeRunner(lis,argc,argv)(s, "AllTests");
}

int main(int argc, char const *argv[]){
    runAllTests(argc,argv);
    return 0;
}
