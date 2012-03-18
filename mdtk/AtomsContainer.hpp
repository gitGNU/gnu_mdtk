/*
   The AtomsContainer class (header file).

   Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2011, 2012
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

#ifndef mdtk_AtomContainer_hpp
#define mdtk_AtomContainer_hpp

#include <mdtk/Atom.hpp>

namespace mdtk
{

class AtomsContainer:public std::vector<Atom*>
{
  std::vector<Atom*> createdAtoms;
public:
  void applyPBC();
  void unfoldPBC();

  void PBC(Vector3D newPBC);
  Vector3D PBC() const;
  bool PBCEnabled() const;

  bool checkMIC(Float Rc) const; // check Minimum Image Criteria
  bool fitInPBC() const;

  void prepareForSimulatation();
  void setAttributesByElementID();

  AtomsContainer();
  AtomsContainer(const AtomsContainer &c);
  AtomsContainer& operator =(const AtomsContainer &c);
  void addAtoms(const AtomsContainer &ac);
  virtual ~AtomsContainer();

  void saveToStream(std::ostream& os, YAATK_FSTREAM_MODE smode);
  void loadFromStream(std::istream& is, YAATK_FSTREAM_MODE smode);

  void normalize();

  Atom* createAtom()
    {
      Atom *pa = new Atom;
      REQUIRE(pa != NULL);
      createdAtoms.push_back(pa);
      return pa;
    };
  Atom* createAtom(const Atom& a)
    {
      Atom *pa = new Atom(a);
      REQUIRE(pa != NULL);
      createdAtoms.push_back(pa);
      return pa;
    };
};

}  // namespace mdtk


#endif


