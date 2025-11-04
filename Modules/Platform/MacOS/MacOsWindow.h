#ifndef POMEGRANATE_MACOSWINDOW_H
#define POMEGRANATE_MACOSWINDOW_H

#include "Platform/Common/StandardWindow.h"


class MacOSWindow : public StandardWindow {
    void* blur = nullptr;
    float blurRadius = 15.0f;
protected:
    void rebuild() override;
public:
    void setBlurRadius(float radius) override;
};


#endif //POMEGRANATE_MACOSWINDOW_H