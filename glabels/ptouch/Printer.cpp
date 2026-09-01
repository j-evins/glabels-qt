//  ptouch/Printer.cpp
//
//  Copyright (C) 2026  gLabels-qt contributors
//  SPDX-License-Identifier: GPL-3.0-or-later

#include "Printer.hpp"

#include "model/PageRenderer.hpp"

#include <QImage>


namespace glabels::ptouch
{

Printer::Printer(Device& device, const DeviceStatus& status, QObject* parent)
    : QObject(parent)
    , mDevice(device)
    , mStatus(status)
{}

int Printer::dpi()            const noexcept { return mDevice.descriptor().dpi;           }
int Printer::maxPixelWidth()  const noexcept { return mDevice.descriptor().maxPixelWidth; }
int Printer::tapePixelWidth() const noexcept { return mStatus.tapePixelWidth;             }

bool Printer::print(const model::PageRenderer& renderer)
{
    mLastError.clear();

    int labelCount = 0;
    int totalLabels = renderer.nItems();

    bool aborted = false;

    renderer.enumerateLabels(static_cast<double>(dpi()),
        [&](const QImage& labelImage) -> bool
        {
            if (labelImage.height() > maxPixelWidth())
            {
                mLastError = QStringLiteral(
                    "Label width (%1 px at %2 DPI) exceeds the print head capacity "
                    "(%3 px).  Please use a narrower template.")
                    .arg(labelImage.height())
                    .arg(dpi())
                    .arg(maxPixelWidth());
                aborted = true;
                return false;   // stop enumeration
            }

            if (!mDevice.printImage(labelImage, mStatus, mLastError))
            {
                aborted = true;
                return false;
            }

            ++labelCount;
            emit progressChanged(labelCount, totalLabels);
            return true;    // continue to next label
        });

    if (!aborted)
    {
        if (!mDevice.eject(mLastError))
        {
            emit finished();
            return false;
        }
    }

    emit finished();
    return !aborted;
}

} // namespace glabels::ptouch
