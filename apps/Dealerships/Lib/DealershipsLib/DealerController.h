#include "stdafx.h"

class DealerController
{
public:
	static DealerController& getInstance();

private:
	DealerController();   

	DealerController(DealerController const&);     
	void operator=(DealerController const&); 

public:

};