//  $Id: translation.hpp 848 2006-10-29 02:14:04Z coz $
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#ifndef __TRANSLATION_H__
#define __TRANSLATION_H__

/** This is not a header providing functional translation support. It just
  * exists to allow using the lisp parser code from SuperTuxKart without
  * changes.
  */

#ifdef HAS_GETTEXT
#  include <libintl.h>
#  define _(String) gettext(String)
#  define gettext_noop(String) String
#  define N_(String) gettext_noop (String)
#else
#  define _(String) (String)
#  define gettext_noop(String) String
#  define N_(String) String
#  define textdomain(Domain)
#  define bindtextdomain(Package, Directory)
#endif

namespace lisp
{
        static const int MAX_ERROR_MESSAGE_LENGTH = 512;

}
#endif

