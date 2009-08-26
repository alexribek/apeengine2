/*
-----------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2006-2008 Henry van Merode

Usage of this program is free for non-commercial use and licensed under the
the terms of the GNU Lesser General Public License.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/

#ifndef __PU_DYNAMIC_ATTRIBUTE_H__
#define __PU_DYNAMIC_ATTRIBUTE_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseIElement.h"
#include "ParticleUniverseCommon.h"

namespace ParticleUniverse
{
	/** Comparer used for sorting vector in ascending order
    */
	struct ControlPointSorter
	{
		bool operator() (const Ogre::Vector2& a, const Ogre::Vector2& b)
		{
			return a.x < b.x;
		}
	};

	/*	The DynamicAttribute class or its child classes encapsulate an attribute with specific (dynamic) behaviour.
	@remarks
		This class provides a uniform interface for retrieving the value of an attribute, while the calculation of
		this value may vary. Each subclass provides its own implementation of the getValue() function and the calling
		application doesn't need to know about the underlying logic. A subclass could just return a value that has 
		previously been set, but it can also return a value that is randomly generated by a pre-defined min/max interval.
		The DynamicAttribute class is used in situations where different behaviour of a certain attribute is needed,
		but where implementation of this behaviour may not be scattered or duplicated within the application that needs 
		it.
	*/
	class _ParticleUniverseExport DynamicAttribute : public IElement
    {
		public:
			enum DynamicAttributeType
			{
				DAT_FIXED,
				DAT_RANDOM,
				DAT_CURVED,
				DAT_OSCILLATE
			};

			/** Constructor
			*/
			DynamicAttribute (void);

			/** Destructor
			*/
			virtual ~DynamicAttribute (void);

			/** Pure virtual function that needs to be implemented by its childs.
			*/
			virtual Ogre::Real getValue (Ogre::Real x = 0) = 0;

			/** 
			*/
			DynamicAttributeType getType (void);

			/** 
			*/
			void setType (DynamicAttributeType type);

			/** 
			*/
			virtual void copyAttributesTo(DynamicAttribute*) = 0;

		protected:
			DynamicAttributeType mType;
	};

	/*	This class is an implementation of the DynamicAttribute class in its most simple form. It just returns a value
		that has previously been set.
	@remarks
		Although use of a regular attribute within the class that needs it is preferred, its benefit is that it makes
		use of the generic 'getValue' mechanism of a DynamicAttribute.
	*/
	class _ParticleUniverseExport DynamicAttributeFixed : public DynamicAttribute
    {
		public:
			/** Constructor
			*/
			DynamicAttributeFixed (void);

			/** Copy constructor
			*/
			DynamicAttributeFixed (const DynamicAttributeFixed& dynamicAttributeFixed);

			/** Destructor
			*/
			~DynamicAttributeFixed (void);

			/** 
			*/
			virtual Ogre::Real getValue (Ogre::Real x = 0);

			/** 
			*/
			virtual void setValue (Ogre::Real value);

			/** 
			*/
			virtual void copyAttributesTo(DynamicAttribute* dynamicAttribute);

		protected:
			Ogre::Real mValue;
	};

	/* This class generates random values within a given minimum and maximum interval.
	*/
	class _ParticleUniverseExport DynamicAttributeRandom : public DynamicAttribute
    {
		public:
			/** Constructor
			*/
			DynamicAttributeRandom (void);

			/** Copy constructor
			*/
			DynamicAttributeRandom (const DynamicAttributeRandom& dynamicAttributeRandom);

			/** Destructor
			*/
			~DynamicAttributeRandom (void);

			/** 
			*/
			virtual Ogre::Real getValue (Ogre::Real x = 0);

			/** 
			*/
			void setMin (Ogre::Real min);
			void setMax (Ogre::Real max);
			void setMinMax (Ogre::Real min, Ogre::Real max);

			/** 
			*/
			virtual void copyAttributesTo(DynamicAttribute* dynamicAttribute);

	protected:
			Ogre::Real mMin, mMax;
	};

	/* This is a more complex usage of the DynamicAttribute principle. This class returns a value on an curve.
	@remarks
		After setting a number of control points, this class is able to interpolate a point on the curve that is based
		on these control points. Interpolation is done in different flavours. �Linear� provides linear interpolation
		of a value on the curve, while �Spline� generates a smooth curve and the returns a value that lies on that curve.
	*/
	class _ParticleUniverseExport DynamicAttributeCurved : public DynamicAttribute
    {
		public:
			typedef std::vector<Ogre::Vector2> ControlPointList;

			/** Constructor
			*/
			DynamicAttributeCurved (void);
			DynamicAttributeCurved (InterpolationType interpolationType);

			/** Copy constructor
			*/
			DynamicAttributeCurved (const DynamicAttributeCurved& dynamicAttributeCurved);

			/** Destructor
			*/
			~DynamicAttributeCurved (void);

			/** Get and set the curve type
			*/
			void setInterpolationType (InterpolationType interpolationType);
			InterpolationType getInterpolationType (void) const;

			/** 
			*/
			virtual Ogre::Real getValue (Ogre::Real x = 0);

			/** 
			*/
			virtual void addControlPoint (Ogre::Real x, Ogre::Real y);

			/** 
			*/
			void processControlPoints (void);

			/** 
			*/
			size_t getNumControlPoints(void) const;

			/** 
			*/
			void removeAllControlPoints(void);

			/** 
			*/
			virtual void copyAttributesTo(DynamicAttribute* dynamicAttribute);

		protected:

			/** 
			*/
			Ogre::Real mRange;

			/** 
			*/
			Ogre::SimpleSpline mSpline;

			/** 
			*/
			InterpolationType mInterpolationType;

			/** 
			*/
			ControlPointList mControlPoints;

			/** Find an iterator that forms the low (left) value of the interval where x lies in.
			*/
			ControlPointList::iterator _findNearestControlPointIterator(Ogre::Real x);

			/** Helper functions 
			*/
			ControlPointList::iterator _getFirstValidIterator(void);
			ControlPointList::iterator _getLastValidIterator(void);
	};

	/* This class generates values based on an oscillating functione (i.e. Sine).
	*/
	class _ParticleUniverseExport DynamicAttributeOscillate : public DynamicAttribute
    {
		public:
			enum OscillationType
			{
				OSCT_SINE,
				OSCT_SQUARE
			};

			/** Constructor
			*/
			DynamicAttributeOscillate (void);

			/** Copy constructor
			*/
			DynamicAttributeOscillate (const DynamicAttributeOscillate& dynamicAttributeOscillate);

			/** Destructor
			*/
			~DynamicAttributeOscillate (void);

			/** 
			*/
			virtual Ogre::Real getValue (Ogre::Real x = 0);

			/** Get and set the OscillationType
			*/
			OscillationType getOscillationType (void) const;
			void setOscillationType (OscillationType oscillationType);

			/** Get and set the Frequency
			*/
			Ogre::Real getFrequency (void) const;
			void setFrequency (Ogre::Real frequency);

			/** Get and set the Phase
			*/
			Ogre::Real getPhase (void) const;
			void setPhase (Ogre::Real phase);

			/** Get and set the Base
			*/
			Ogre::Real getBase (void) const;
			void setBase (Ogre::Real base);

			/** Get and set the Amplitude
			*/
			Ogre::Real getAmplitude (void) const;
			void setAmplitude (Ogre::Real amplitude);

			/** 
			*/
			virtual void copyAttributesTo(DynamicAttribute* dynamicAttribute);

		protected:
			OscillationType mOscillationType;
			Ogre::Real  mFrequency;
			Ogre::Real  mPhase;
			Ogre::Real  mBase;
			Ogre::Real  mAmplitude;
	};

	/* Helper class to do some generic calculation.
	*/
	class _ParticleUniverseExport DynamicAttributeHelper
	{
		public:
			/* Return the value of a DynamicAttribute, given te x value.
			*/
			Ogre::Real calculate(DynamicAttribute* dyn, Ogre::Real x, Ogre::Real defaultValue = 0.0f);
	};

};
#endif
