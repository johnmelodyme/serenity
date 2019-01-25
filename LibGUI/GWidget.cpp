#include "GWidget.h"
#include "GEvent.h"
#include "GEventLoop.h"
#include "GWindow.h"
#include <AK/Assertions.h>
#include <SharedGraphics/GraphicsBitmap.h>
#include <SharedGraphics/Painter.h>

GWidget::GWidget(GWidget* parent)
    : GObject(parent)
{
    set_font(nullptr);
    m_background_color = Color::White;
    m_foreground_color = Color::Black;
}

GWidget::~GWidget()
{
}

void GWidget::set_relative_rect(const Rect& rect)
{
    if (rect == m_relative_rect)
        return;
    // FIXME: Make some kind of event loop driven ResizeEvent?
    m_relative_rect = rect;
    update();
}

void GWidget::repaint(const Rect& rect)
{
    // FIXME: Implement.
}

void GWidget::event(GEvent& event)
{
    switch (event.type()) {
    case GEvent::Paint:
        m_has_pending_paint_event = false;
        return paint_event(static_cast<GPaintEvent&>(event));
    case GEvent::Show:
        return show_event(static_cast<GShowEvent&>(event));
    case GEvent::Hide:
        return hide_event(static_cast<GHideEvent&>(event));
    case GEvent::KeyDown:
        return keydown_event(static_cast<GKeyEvent&>(event));
    case GEvent::KeyUp:
        return keyup_event(static_cast<GKeyEvent&>(event));
    case GEvent::MouseMove:
        return mousemove_event(static_cast<GMouseEvent&>(event));
    case GEvent::MouseDown:
        // FIXME: Focus self if needed.
        return mousedown_event(static_cast<GMouseEvent&>(event));
    case GEvent::MouseUp:
        return mouseup_event(static_cast<GMouseEvent&>(event));
    default:
        return GObject::event(event);
    }
}

void GWidget::paint_event(GPaintEvent& event)
{
    if (fill_with_background_color()) {
        Painter painter(*this);
        painter.fill_rect(event.rect(), background_color());
    }
    for (auto* ch : children()) {
        auto* child = (GWidget*)ch;
        child->event(event);
    }
}

void GWidget::show_event(GShowEvent&)
{
}

void GWidget::hide_event(GHideEvent&)
{
}

void GWidget::keydown_event(GKeyEvent&)
{
}

void GWidget::keyup_event(GKeyEvent&)
{
}

void GWidget::mousedown_event(GMouseEvent&)
{
}

void GWidget::mouseup_event(GMouseEvent&)
{
}

void GWidget::mousemove_event(GMouseEvent&)
{
}

void GWidget::update()
{
    auto* w = window();
    if (!w)
        return;
    if (m_has_pending_paint_event)
        return;
    m_has_pending_paint_event = true;
    GEventLoop::main().post_event(w, make<GPaintEvent>(relative_rect()));
}

GWidget::HitTestResult GWidget::hit_test(int x, int y)
{
    // FIXME: Care about z-order.
    for (auto* ch : children()) {
        auto* child = (GWidget*)ch;
        if (child->relative_rect().contains(x, y)) {
            return child->hit_test(x - child->relative_rect().x(), y - child->relative_rect().y());
        }
    }
    return { this, x, y };
}

void GWidget::set_window(GWindow* window)
{
    if (m_window == window)
        return;
    m_window = window;
}

bool GWidget::is_focused() const
{
    // FIXME: Implement.
    return false;
}

void GWidget::set_focus(bool focus)
{
    if (focus == is_focused())
        return;
    // FIXME: Implement.
}

void GWidget::set_font(RetainPtr<Font>&& font)
{
    if (!font)
        m_font = Font::default_font();
    else
        m_font = move(font);
}