//**************************************************************************
//
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//
//**************************************************************************
//
//  Filename   :  IRecipe.h
//  
//  Subsystem  :  Axeda Connector
//
//  Description:  Recipe interface definitions
//
//  Classes:
//          IRecipe
//          IRecipeMgr
//
//**************************************************************************
#ifndef __Recipe_INTERFACE_H__
#define __Recipe_INTERFACE_H__

/* Group=Recipe */

//**************************************************************************
//
struct ERecipeValueDef
{
    // Use extra information to distinguish strings which are data item
    // names from strings which are just a constant value
    CDataValue::ValueType  enType;
    bool                   m_bValueFromDataItem;
    TCHAR*                 pszValue; // string representation of value or name
};

//**************************************************************************
//
struct ERecipeRowDef
{
    TCHAR*           pszRowDataItemName;// name of data item to write to
    int              nCols;             // column count for this recipe
    ERecipeValueDef* pRecipeValues;     // array of values, one per column
};

//**************************************************************************
//
struct ERecipeDef
{
    int            nRows;      // Number of Rows in recipe
    int            nCols;      // Number of Columns in this recipe
    TCHAR*         pszName;    // Recipe name, used by recipe manager
    ERecipeRowDef* pRecipeRows;// array of rows
};

//**************************************************************************
//
interface IRecipe
{
    virtual KERESULT    AddRow(ERecipeRowDef &newRow) = 0; // Add a new row to the end
    virtual KERESULT    RemoveRow(int nRowNum) = 0; // Remove row at specified number
    virtual KERESULT    RemoveColumn(int nColNum) = 0; // Remove specified column from all rows
    virtual KERESULT    GetRowNumByName(TCHAR* pszDataItemName, int &nRowNum) = 0; // Specify a data item name, find the number of the row writing to that data item.
    virtual KERESULT    GetRowsSize(int& nNumRows) = 0; // Number of Rows in Recipe
    virtual KERESULT    SetValueAt(int nColNum, int nRowNum, ERecipeValueDef value) = 0; // Change the value used at a specific column and row
    virtual KERESULT    SetRowDataItem(int nRowNum, TCHAR* pszDataItemName) = 0; // Tell a Row to write to the named DataItem

};

//**************************************************************************
//
interface IRecipeMgr : public IKernelBase
{
    virtual KERESULT AddRecipe(ERecipeDef& rRecipe) = 0;
    virtual KERESULT RemoveRecipe(TCHAR* pszName) = 0;
};

#endif //__Recipe_INTERFACE_H__
