#ifndef POMEGRANATE_MACOSWINDOW_H
#define POMEGRANATE_MACOSWINDOW_H

#include "Platform/Common/StandardWindow.h"


class MacOSWindow : public StandardWindow {
public:
    void setBlurRadius(float radius) override;
    float getBlurRadius() override;
protected:
    void rebuild() override;
private:
    void* blur = nullptr;
    float blurRadius = 15.0f;
};


#endif //POMEGRANATE_MACOSWINDOW_H