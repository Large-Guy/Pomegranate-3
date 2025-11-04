#include "Effects.hpp"

#include "SDL3/SDL.h"
#include "SDL3/SDL_video.h"
#include <stdexcept>
#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>

@interface ConfigurableBlurView : NSVisualEffectView
@property (nonatomic, assign) CGFloat blurRadius;
@end

@implementation ConfigurableBlurView

- (instancetype)initWithFrame:(NSRect)frameRect {
    self = [super initWithFrame:frameRect];
    if (self) {
        _blurRadius = 10.0;
    }
    return self;
}

- (void)updateLayer {
    [super updateLayer];
    
    [CATransaction begin];
    [CATransaction setDisableActions:YES];
    
    // Original code attempting to find blur
    CALayer *materialLayer = self.layer.sublayers.firstObject;
    if (materialLayer) {
        for (CALayer *sublayer in materialLayer.sublayers) {
            if ([sublayer.name isEqualToString:@"backdrop"]) {
                // Found the backdrop layer with filters
                for (id filter in sublayer.filters) {
                    if ([filter respondsToSelector:@selector(name)] && 
                        [[filter name] isEqualToString:@"gaussianBlur"]) {
                        // Found it! Set the blur radius
                        if ([filter respondsToSelector:@selector(setValue:forKey:)]) {
                            [filter setValue:@(self.blurRadius) forKey:@"inputRadius"];
                        }
                    }
                }
            }
        }
    }
    
    [CATransaction commit];
}

@end

void* createVibrancy(SDL_Window *window, float intensity) {
    SDL_PropertiesID properties = SDL_GetWindowProperties(window);
    auto *win = (__bridge NSWindow *) SDL_GetPointerProperty(properties, "SDL.window.cocoa.window", nullptr);

    if (!win)
        throw std::runtime_error("Cocoa window was not valid!");

    win.opaque = NO;
    win.backgroundColor = [NSColor clearColor];

    ConfigurableBlurView *blurView = [[ConfigurableBlurView alloc] initWithFrame:win.contentView.bounds];
    blurView.blurRadius = intensity;
    blurView.material = NSVisualEffectMaterialUnderWindowBackground;
    blurView.blendingMode = NSVisualEffectBlendingModeWithinWindow;
    blurView.state = NSVisualEffectStateActive;
    blurView.translatesAutoresizingMaskIntoConstraints = NO; 

    [win.contentView addSubview:blurView positioned:NSWindowBelow relativeTo:nil];

    [NSLayoutConstraint activateConstraints:@[
            [blurView.leadingAnchor constraintEqualToAnchor:win.contentView.leadingAnchor],
            [blurView.trailingAnchor constraintEqualToAnchor:win.contentView.trailingAnchor],
            [blurView.topAnchor constraintEqualToAnchor:win.contentView.topAnchor],
            [blurView.bottomAnchor constraintEqualToAnchor:win.contentView.bottomAnchor]
    ]];
    
    return (void *) blurView;
}

void destroyVibrancy(void* blur) {
    auto* blurView = (__bridge ConfigurableBlurView *) blur;
    blurView = nil;
}