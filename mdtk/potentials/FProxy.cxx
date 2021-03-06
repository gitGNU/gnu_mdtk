/*
   The proxy class for interatomic potentials.

   Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2012, 2013
   Oleksandr Yermolenko <oleksandr.yermolenko@gmail.com>

   This file is part of MDTK, the Molecular Dynamics Toolkit.

   MDTK is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   MDTK is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with MDTK.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FProxy.hpp"
#include "typeinfo"

namespace mdtk
{

FProxy::FProxy()
 :potentials()
{
}

Float
FProxy::getRcutoff() const
{
  REQUIRE(potentials.size() > 0);
  Float maxv = potentials[0]->getRcutoff();
  for(size_t i = 0; i < potentials.size(); i++)
    if (potentials[i]->getRcutoff() > maxv) maxv = potentials[i]->getRcutoff();
  return maxv;
}  


Float
FProxy::operator()(AtomsArray& gl)
{
  Float Ei = 0;

  for(size_t i = 0; i < gl.size(); i++)
    gl[i].grad = Vector3D(0,0,0);

  for(size_t i = 0; i < potentials.size(); i++)
  {
    Float Ecur = (*(potentials[i]))(gl);
    Ei += Ecur;
    {Float E = Ecur/eV;PRINT("E" << i << " : " << E << "\n");}
  }

  return Ei;
}

void
FProxy::diagnose()
{
  PRINT("Interatomic potentials:\n");
  for(size_t i = 0; i < potentials.size(); i++)
  {
    TRACE(typeid(*(potentials[i])).name());
    TRACE(potentials[i]->getRcutoff()/Ao);
  }
}

} // namespace mdtk


