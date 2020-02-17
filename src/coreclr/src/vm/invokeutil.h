// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
//

//
////////////////////////////////////////////////////////////////////////////////
// This module defines a Utility Class used by reflection
//
//

////////////////////////////////////////////////////////////////////////////////


#ifndef __INVOKEUTIL_H__
#define __INVOKEUTIL_H__

// The following class represents the value class
#include <pshpack1.h>

struct InterfaceMapData
{
    REFLECTCLASSBASEREF     m_targetType;
    REFLECTCLASSBASEREF     m_interfaceType;
    PTRARRAYREF             m_targetMethods;
    PTRARRAYREF             m_interfaceMethods;
};

// Calling Conventions
// NOTE: These are defined in CallingConventions.cs They must match up.
#define Standard_CC     0x0001
#define VarArgs_CC      0x0002
#define Any_CC          (Standard_CC | VarArgs_CC)

#define PRIMITIVE_TABLE_SIZE  ELEMENT_TYPE_STRING
#define PT_Primitive    0x01000000

// Define the copy back constants.
#define COPYBACK_PRIMITIVE      1
#define COPYBACK_OBJECTREF      2
#define COPYBACK_VALUECLASS     3

#include <poppack.h>

class ArgDestination;

// This class abstracts the functionality which creats the
//  parameters on the call stack and deals with the return type
//  inside reflection.
//
class InvokeUtil
{

public:
    static void CopyArg(TypeHandle th, OBJECTREF *obj, ArgDestination *argDest);

    // Given a type, this routine will convert an return value representing that
    //  type into an ObjectReference.  If the type is a primitive, the
    //  value is wrapped in one of the Value classes.
    static OBJECTREF CreateObjectAfterInvoke(TypeHandle th, void * pValue);

    // This is a special purpose Exception creation function.  It
    //  creates the TargetInvocationExeption placing the passed
    //  exception into it.
    static OBJECTREF CreateTargetExcept(OBJECTREF* except);

    // This is a special purpose Exception creation function.  It
    //  creates the ReflectionClassLoadException placing the passed
    //  classes array and exception array into it.
    static OBJECTREF CreateClassLoadExcept(OBJECTREF* classes,OBJECTREF* except);

    // Validate that the field can be widened for Set
    static void ValidField(TypeHandle th, OBJECTREF* value);

    // ChangeType
    // This method will invoke the Binder change type method on the object
    //  binder -- The Binder object
    //  srcObj -- The source object to be changed
    //  th -- The TypeHandel of the target type
    //  locale -- The locale passed to the class.
    static OBJECTREF ChangeType(OBJECTREF binder,OBJECTREF srcObj,TypeHandle th,OBJECTREF locale);

    // CreatePrimitiveValue
    // This routine will validate the object and then place the value into
    //  the destination
    //  dstType -- The type of the destination
    //  srcType -- The type of the source
    //  srcObj -- The Object containing the primitive value.
    //  pDst -- poiner to the destination
    static void CreatePrimitiveValue(CorElementType dstType, CorElementType srcType, OBJECTREF srcObj, ARG_SLOT* pDst);

    // CreatePrimitiveValue
    // This routine will validate the object and then place the value into
    //  the destination
    //  dstType -- The type of the destination
    //  srcType -- The type of the source
    //  pSrc -- pointer to source data.
    //  pSrcMT - MethodTable of source type
    //  pDst -- poiner to the destination
    static void CreatePrimitiveValue(CorElementType dstType,CorElementType srcType,
        void *pSrc, MethodTable *pSrcMT, ARG_SLOT* pDst);

    // IsPrimitiveType
    // This method will verify the passed in type is a primitive or not
    //	type -- the CorElementType to check for
    inline static DWORD IsPrimitiveType(const CorElementType type)
    {
        LIMITED_METHOD_CONTRACT;

        if (type >= PRIMITIVE_TABLE_SIZE)
        {
            if (ELEMENT_TYPE_I==type || ELEMENT_TYPE_U==type)
            {
                return TRUE;
            }
            return 0;
        }

        return (PT_Primitive & PrimitiveAttributes[type]);
    }

    static BOOL IsVoidPtr(TypeHandle th);

    // CanPrimitiveWiden
    // This method determines if the srcType and be widdened without loss to the destType
    //  destType -- The target type
    //  srcType -- The source type.
    inline static DWORD CanPrimitiveWiden(const CorElementType destType, const CorElementType srcType)
    {
        LIMITED_METHOD_CONTRACT;

        if (destType >= PRIMITIVE_TABLE_SIZE || srcType >= PRIMITIVE_TABLE_SIZE)
        {
            if ((ELEMENT_TYPE_I==destType && ELEMENT_TYPE_I==srcType) ||
                (ELEMENT_TYPE_U==destType && ELEMENT_TYPE_U==srcType))
            {
                return TRUE;
            }
            return 0;
        }
        return ((1 << destType) & PrimitiveAttributes[srcType]);
    }

    // Field Stuff.  The following stuff deals with fields making it possible
    //  to set/get field values on objects

    // SetValidField
    // Given an target object, a value object and a field this method will set the field
    //  on the target object.  The field must be validate before calling this.
    static void SetValidField(CorElementType fldType, TypeHandle fldTH, FieldDesc* pField, OBJECTREF* target, OBJECTREF* value, TypeHandle declaringType, CLR_BOOL *pDomainInitialized);

    static OBJECTREF GetFieldValue(FieldDesc* pField, TypeHandle fieldType, OBJECTREF* target, TypeHandle declaringType, CLR_BOOL *pDomainInitialized);

    // ValidateObjectTarget
    // This method will validate the Object/Target relationship
    //  is correct.  It throws an exception if this is not the case.
    static void ValidateObjectTarget(FieldDesc* pField,TypeHandle fldType,OBJECTREF *target);

    // Create reflection pointer wrapper
    static OBJECTREF CreatePointer(TypeHandle th, void * p);

    static TypeHandle GetPointerType(OBJECTREF pObj);
    static void* GetPointerValue(OBJECTREF pObj);
    static void* GetIntPtrValue(OBJECTREF pObj);

private:
    static void* CreateByRef(TypeHandle dstTh,CorElementType srcType, TypeHandle srcTH,OBJECTREF srcObj, OBJECTREF *pIncomingObj);

    // GetBoxedObject
    // Given an address of a primitve type, this will box that data...
    static OBJECTREF GetBoxedObject(TypeHandle th,void* pData);

private:
    // The Attributes Table
    // This constructs a table of legal widening operations
    //  for the primitive types.
    static DWORD const PrimitiveAttributes[PRIMITIVE_TABLE_SIZE];
};


#endif // __INVOKEUTIL_H__
