/*
 * This file is part of aura
 *
 * Copyright (C) 2012 Igalia S.L.
 *
 * Contact: Miguel Gómez <magomez@igalia.com>
 *          Xabier Rodriguez Calvar <xrcalvar@igalia.com>
 *          Víctor Jáquez <vjaquez@igalia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#ifndef CONTROLLERSETTINGS_H
#define CONTROLLERSETTINGS_H

#include <QDeclarativeItem>

class ControllerSettings : public QDeclarativeItem
{
    Q_OBJECT
    Q_ENUMS(ColorFilter)
    Q_ENUMS(Device)

 public:
    // colour filter values
    enum ColorFilter {
        Normal = 0,
        Sepia,
        Negative,
        Grayscale,
        Natural,
        Vivid,
        ColorSwap,
        Solarize
    };

    enum Device {
        Primary = 0,
        Secondary
    };
};

#endif
