// gcc -o click click.c -Wall -framework ApplicationServices
// example:  https://github.com/go-vgo/robotgo/blob/master/mouse/mouse_c.h
// reference: https://developer.apple.com/documentation/coregraphics/1455788-cgeventgetlocation
#include <ApplicationServices/ApplicationServices.h>
#include <unistd.h>

#define FIVE_MINUTES 300

CGPoint location()
{
    CGEventRef event = CGEventCreate(NULL);
    CGPoint point = CGEventGetLocation(event);
    CFRelease(event);

    return point;
}

void calculateDeltas(CGEventRef *event, CGPoint point)
{
    CGEventRef get = CGEventCreate(NULL);
    CGPoint mouse = CGEventGetLocation(get);

    int64_t deltaX = point.x - mouse.x;
    int64_t deltaY = point.y - mouse.y;

    CGEventSetIntegerValueField(*event, kCGMouseEventDeltaX, deltaX);
    CGEventSetIntegerValueField(*event, kCGMouseEventDeltaY, deltaY);

    CFRelease(get);
}

void moveMouse(CGPoint point)
{
    CGEventRef move = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved, point, kCGMouseButtonLeft); //kCGMouseButonLeft is ignored unless event is kCGEventOtherMouseDown, kCGEventOtherMouseDragged, or kCGEventOtherMouseUp. https://developer.apple.com/documentation/coregraphics/1454356-cgeventcreatemouseevent
    calculateDeltas(&move, point);
    CGEventPost(kCGSessionEventTap, move);
    CFRelease(move);
}

void exampleClick()
{
    const CGFloat X = 422;
    const CGFloat Y = 192;

    CGEventRef click1_down = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, CGPointMake(X, Y), kCGMouseButtonLeft);

    CGEventRef click1_up = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, CGPointMake(X, Y), kCGMouseButtonLeft);

    for (int i = 0; i < 100; i++) {
        CGEventPost(kCGHIDEventTap, click1_down);
        sleep(1);
        CGEventPost(kCGHIDEventTap, click1_up);
        sleep(1);
    }

    CFRelease(click1_up);
    CFRelease(click1_down);
}

int main()
{
    while (1) {
        moveMouse(location());
        sleep(FIVE_MINUTES);
    }

    return 0;
}
