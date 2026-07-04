//  ptouch/Printer.hpp
//
//  High-level label printer: drives a Device from a model::PageRenderer.
//
//  Typical use (from PrintView):
//
//    ptouch::Printer printer(*device, deviceStatus);
//    printer.print(renderer);
//    if (printer.lastError().isEmpty()) { ... }
//
//  Copyright (C) 2026  gLabels-qt contributors
//  SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Device.hpp"

#include <QObject>
#include <QString>


namespace glabels::model { class PageRenderer; }


namespace glabels::ptouch
{

/// Bridges a model::PageRenderer to a P-Touch Device.
///
/// Iterates over every label item produced by the renderer, renders each one
/// to a QImage at the device's native DPI, and submits it to the device via
/// Device::printImage().  After the last label the tape is ejected and cut.
///
/// All operations are synchronous and happen on the calling thread.
class Printer : public QObject
{
    Q_OBJECT

public:
    /// Construct a Printer for @p device.  @p status must be a freshly
    /// obtained DeviceStatus (call Device::queryStatus() immediately before).
    explicit Printer(Device& device, const DeviceStatus& status,
                     QObject* parent = nullptr);

    // ── Print ────────────────────────────────────────────────────────────────

    /// Render and print every label item from @p renderer to the device.
    /// Returns true on success.  On failure lastError() contains a description.
    ///
    /// Emits progressChanged() for each label submitted and finished() at the end.
    bool print(const model::PageRenderer& renderer);

    // ── Result ───────────────────────────────────────────────────────────────

    /// The last error message, or an empty string if the last print succeeded.
    QString lastError() const { return mLastError; }

    // ── Device info ──────────────────────────────────────────────────────────

    /// Native device DPI (used to scale labels to pixels).
    int dpi() const noexcept;

    /// Maximum pixel width of the print head.
    int maxPixelWidth() const noexcept;

    /// Printable pixel width of the loaded tape.
    int tapePixelWidth() const noexcept;

signals:
    /// Emitted after each label image is sent to the device.
    /// @p completed is 1-based; @p total is the total label count.
    void progressChanged(int completed, int total);

    /// Emitted when all labels have been sent (whether or not there was an error).
    void finished();

private:
    Device&      mDevice;
    DeviceStatus mStatus;
    QString      mLastError;
};

} // namespace glabels::ptouch
