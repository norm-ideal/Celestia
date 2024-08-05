// qtdeepskybrowser.h
//
// Copyright (C) 2023, Celestia Development Team
//
// Drag handler for Qt5+ front-end.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#pragma once

#include <memory>

#include <QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QPoint>
#else
#include <QPointF>
#endif

class QMouseEvent;
class QWidget;

class CelestiaCore;

namespace celestia::qt
{

// The base version of DragHandler is the fallback implementation for
// platforms which do not support pointer warping
class DragHandler
{
public:
    explicit DragHandler(CelestiaCore *core) : appCore(core)
    {
    }
    virtual ~DragHandler() = default;

    DragHandler(const DragHandler &)            = delete;
    DragHandler &operator=(const DragHandler &) = delete;
    DragHandler(DragHandler &&)                 = delete;
    DragHandler &operator=(DragHandler &&)      = delete;

    virtual void begin(const QMouseEvent &, qreal, int);
    virtual void move(const QMouseEvent &, qreal);
    virtual void finish(){ /* nothing to do */ };

    void setButton(int);
    void clearButton(int);

protected:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    using PointType = QPoint;
#else
    using PointType = QPointF;
#endif

    CelestiaCore *appCore;
    PointType     saveCursorPos{};
    qreal         scale{};
    int           buttons{ 0 };

    int effectiveButtons() const;
};

// Implementation of DragHandler which uses pointer warping to enable infinite
// movement
class WarpingDragHandler : public DragHandler
{
public:
    using DragHandler::DragHandler;

    void move(const QMouseEvent &, qreal) override;
    void finish() override;

private:
    void restoreCursorPosition() const;
};

std::unique_ptr<DragHandler> createDragHandler(QWidget *, CelestiaCore *);

} // end namespace celestia::qt
