#include "Display/ClickLayer.h"

#include "Profiler/Profiler.h"

ClickLayer::ClickLayer(function<void()> rf) : _resetFunc(rf)
{
    setWantsKeyboardFocus(true);
}

void ClickLayer::mouseDown(const MouseEvent& event)
{
    if (_resetFunc) { _resetFunc(); }
}

void ClickLayer::mouseDoubleClick(const MouseEvent& e)
{
    JUCEApplicationBase::quit();
}