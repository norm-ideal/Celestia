/*
 *  Celestia GTK+ Front-End
 *  Copyright (C) 2005 Pat Suwalski <pat@suwalski.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  $Id: menu-context.h,v 1.1 2005-12-06 03:19:35 suwalski Exp $
 */

#pragma once

#include <gtk/gtk.h>

#include <celengine/body.h>
#include <celengine/selection.h>
#include <celestia/celestiacore.h>

#include "common.h"

namespace celestia::gtk
{

class GTKContextMenuHandler : public CelestiaCore::ContextMenuHandler
{
 public:
    GTKContextMenuHandler(AppData* app);

    void requestContextMenu(float, float, Selection sel);
};

} // end namespace celestia::gtk
