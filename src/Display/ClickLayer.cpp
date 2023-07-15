#include "Display/ClickLayer.h"

#include "Profiler/Profiler.h"

ClickLayer::ClickLayer(RaceTimer* time, LapCounter* lap, MapComponent* map) : 
    _timer(time), _lapCount(lap), _mapComp(map)
{

}

void ClickLayer::mouseDown(const MouseEvent& event)
{
    _timer->reset();
    _lapCount->reset();
    _mapComp->reset();
    
}

void ClickLayer::mouseDoubleClick(const MouseEvent& e)
{
    JUCEApplicationBase::quit();
}