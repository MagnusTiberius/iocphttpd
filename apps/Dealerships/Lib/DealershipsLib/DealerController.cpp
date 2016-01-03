#include "stdafx.h"
#include "DealerController.h"


DealerController::DealerController()
{
}

DealerController& DealerController::getInstance()
{
	static DealerController    instance;
	return instance;
}

