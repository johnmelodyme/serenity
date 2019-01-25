#pragma once

#include "GObject.h"
#include <SharedGraphics/Rect.h>
#include <SharedGraphics/GraphicsBitmap.h>
#include <AK/AKString.h>

class GWidget;

class GWindow final : public GObject {
public:
    GWindow(GObject* parent = nullptr);
    virtual ~GWindow() override;

    static GWindow* from_window_id(int);

    int window_id() const { return m_window_id; }

    void set_title(String&&);

    int x() const { return rect().x(); }
    int y() const { return rect().y(); }
    int width() const { return rect().width(); }
    int height() const { return rect().height(); }

    Rect rect() const;
    void set_rect(const Rect&);

    Point position() const { return rect().location(); }

    virtual void event(GEvent&) override;

    bool is_visible() const;

    void close();

    GWidget* main_widget() { return m_main_widget; }
    const GWidget* main_widget() const { return m_main_widget; }
    void set_main_widget(GWidget*);

    void show();

    void update();

private:
    RetainPtr<GraphicsBitmap> m_backing;
    int m_window_id { -1 };
    GWidget* m_main_widget { nullptr };
};
