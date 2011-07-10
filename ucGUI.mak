#all:
#	arm-linux-g++ -O2 -march=armv4 -DLinux -lrt -o main *.cpp *.c

#clean:
#	rm main

#WIN32 = WINXP

ifndef WIN32

LINUX = X_GUI

OUTDIR = OUTPUT
OBJDIR = $(OUTDIR)/OBJ
LIBDIR = $(OUTDIR)/LIB

CPP=arm-linux-g++
CP=arm-linux-gcc
MKLIB=arm-linux-ar r

SYSINC=-I/usr/local/arm/3.4.6/arm-linux/usr/include \
	 -I/usr/local/arm/3.4.6/arm-linux/usr/X11R6/include \
	-I/usr/local/arm/3.4.6/arm-linux/usr/include/libxml2

CFLAG=-O2 $(SYSINC) -march=armv4 -I./Config -I./GUI/Core -I./GUI/WM -I./GUI/Widget -DLinux -c
CPPFLAG=-O2 $(SYSINC) -march=armv4 -I./Config -I./GUI/Core -I./GUI/WM -I./GUI/Widget -DLinux -c
LDFLAG=-lrt

AAOBJS = $(OBJDIR)/GUIAAArc.o \
	$(OBJDIR)/GUIAAChar2.o \
	$(OBJDIR)/GUIAAChar4.o \
	$(OBJDIR)/GUIAAChar.o \
	$(OBJDIR)/GUIAACircle.o \
	$(OBJDIR)/GUIAALib.o \
	$(OBJDIR)/GUIAALine.o \
	$(OBJDIR)/GUIAAPoly.o \
	$(OBJDIR)/GUIAAPolyOut.o

CCOBJS = $(OBJDIR)/LCDP111.o \
	$(OBJDIR)/LCDP222.o \
	$(OBJDIR)/LCDP233.o \
	$(OBJDIR)/LCDP323.o \
	$(OBJDIR)/LCDP332.o \
	$(OBJDIR)/LCDP444_12.o \
	$(OBJDIR)/LCDP444_12_1.o \
	$(OBJDIR)/LCDP444_16.o \
	$(OBJDIR)/LCDP555.o \
	$(OBJDIR)/LCDP556.o \
	$(OBJDIR)/LCDP565.o \
	$(OBJDIR)/LCDP655.o \
	$(OBJDIR)/LCDP666.o \
	$(OBJDIR)/LCDP888.o \
	$(OBJDIR)/LCDP8666.o \
	$(OBJDIR)/LCDP8666_1.o \
	$(OBJDIR)/LCDPM233.o \
	$(OBJDIR)/LCDPM323.o \
	$(OBJDIR)/LCDPM332.o \
	$(OBJDIR)/LCDPM444_12.o \
	$(OBJDIR)/LCDPM444_16.o \
	$(OBJDIR)/LCDPM555.o \
	$(OBJDIR)/LCDPM556.o \
	$(OBJDIR)/LCDPM565.o \
	$(OBJDIR)/LCDPM655.o \
	$(OBJDIR)/LCDPM666.o \
	$(OBJDIR)/LCDPM888.o

CMOBJS = $(OBJDIR)/LCDP0.o \
	$(OBJDIR)/LCDP2.o \
	$(OBJDIR)/LCDP4.o

COREOBJS = $(OBJDIR)/GUI2DLib.o \
	$(OBJDIR)/GUI__AddSpaceHex.o \
	$(OBJDIR)/GUI__Arabic.o \
	$(OBJDIR)/GUI__CalcTextRect.o \
	$(OBJDIR)/GUI__DivideRound32.o \
	$(OBJDIR)/GUI__DivideRound.o \
	$(OBJDIR)/GUI__DrawBitmap16bpp.o \
	$(OBJDIR)/GUI__GetCursorPos.o \
	$(OBJDIR)/GUI__GetFontSizeY.o \
	$(OBJDIR)/GUI__GetNumChars.o \
	$(OBJDIR)/GUI__HandleEOLine.o \
	$(OBJDIR)/GUI__IntersectRect.o \
	$(OBJDIR)/GUI__IntersectRects.o \
	$(OBJDIR)/GUI__memset16.o \
	$(OBJDIR)/GUI__memset.o \
	$(OBJDIR)/GUI__Read.o \
	$(OBJDIR)/GUI__ReduceRect.o \
	$(OBJDIR)/GUI__SetText.o \
	$(OBJDIR)/GUI__strcmp.o \
	$(OBJDIR)/GUI__strlen.o \
	$(OBJDIR)/GUI__Wrap.o \
	$(OBJDIR)/GUI_AddBin.o \
	$(OBJDIR)/GUI_AddDec.o \
	$(OBJDIR)/GUI_AddDecMin.o \
	$(OBJDIR)/GUI_AddDecShift.o \
	$(OBJDIR)/GUI_AddHex.o \
	$(OBJDIR)/GUI_AddKeyMsgHook.o \
	$(OBJDIR)/GUI_ALLOC_AllocInit.o \
	$(OBJDIR)/GUI_ALLOC_AllocZero.o \
	$(OBJDIR)/GUI_BMP.o \
	$(OBJDIR)/GUI_BMP_Serialize.o \
	$(OBJDIR)/GUI_CalcColorDist.o \
	$(OBJDIR)/GUI_ClearRectEx.o \
	$(OBJDIR)/GUI_Color2VisColor.o \
	$(OBJDIR)/GUI_CursorArrowL.o \
	$(OBJDIR)/GUI_CursorArrowLI.o \
	$(OBJDIR)/GUI_CursorArrowLPx.o \
	$(OBJDIR)/GUI_CursorArrowM.o \
	$(OBJDIR)/GUI_CursorArrowMI.o \
	$(OBJDIR)/GUI_CursorArrowMPx.o \
	$(OBJDIR)/GUI_CursorArrowS.o \
	$(OBJDIR)/GUI_CursorArrowSI.o \
	$(OBJDIR)/GUI_CursorArrowSPx.o \
	$(OBJDIR)/GUI_CursorCrossL.o \
	$(OBJDIR)/GUI_CursorCrossLI.o \
	$(OBJDIR)/GUI_CursorCrossLPx.o \
	$(OBJDIR)/GUI_CursorCrossM.o \
	$(OBJDIR)/GUI_CursorCrossMI.o \
	$(OBJDIR)/GUI_CursorCrossMPx.o \
	$(OBJDIR)/GUI_CursorCrossS.o \
	$(OBJDIR)/GUI_CursorCrossSI.o \
	$(OBJDIR)/GUI_CursorCrossSPx.o \
	$(OBJDIR)/GUI_CursorHeaderM.o \
	$(OBJDIR)/GUI_CursorHeaderMI.o \
	$(OBJDIR)/GUI_CursorHeaderMPx.o \
	$(OBJDIR)/GUI_CursorPal.o \
	$(OBJDIR)/GUI_CursorPalI.o \
	$(OBJDIR)/GUI_DispBin.o \
	$(OBJDIR)/GUI_DispCEOL.o \
	$(OBJDIR)/GUI_DispChar.o \
	$(OBJDIR)/GUI_DispChars.o \
	$(OBJDIR)/GUI_DispHex.o \
	$(OBJDIR)/GUI_DispString.o \
	$(OBJDIR)/GUI_DispStringAt.o \
	$(OBJDIR)/GUI_DispStringAtCEOL.o \
	$(OBJDIR)/GUI_DispStringHCenter.o \
	$(OBJDIR)/GUI_DispStringInRect.o \
	$(OBJDIR)/GUI_DispStringInRectEx.o \
	$(OBJDIR)/GUI_DispStringInRectWrap.o \
	$(OBJDIR)/GUI_DispStringLen.o \
	$(OBJDIR)/GUI_DrawBitmap.o \
	$(OBJDIR)/GUI_DrawBitmap_555.o \
	$(OBJDIR)/GUI_DrawBitmap_565.o \
	$(OBJDIR)/GUI_DrawBitmap_888.o \
	$(OBJDIR)/GUI_DrawBitmapEx.o \
	$(OBJDIR)/GUI_DrawBitmapExp.o \
	$(OBJDIR)/GUI_DrawBitmapMag.o \
	$(OBJDIR)/GUI_DrawFocusRect.o \
	$(OBJDIR)/GUI_DrawGraph.o \
	$(OBJDIR)/GUI_DrawGraphEx.o \
	$(OBJDIR)/GUI_DrawHLine.o \
	$(OBJDIR)/GUI_DrawLine.o \
	$(OBJDIR)/GUI_DrawPie.o \
	$(OBJDIR)/GUI_DrawPixel.o \
	$(OBJDIR)/GUI_DrawPoint.o \
	$(OBJDIR)/GUI_DrawPolygon.o \
	$(OBJDIR)/GUI_DrawPolyline.o \
	$(OBJDIR)/GUI_DrawRectEx.o \
	$(OBJDIR)/GUI_DrawVLine.o \
	$(OBJDIR)/GUI_ErrorOut.o \
	$(OBJDIR)/GUI_Exec.o \
	$(OBJDIR)/GUI_FillPolygon.o \
	$(OBJDIR)/GUI_FillRect.o \
	$(OBJDIR)/GUI_FillRectEx.o \
	$(OBJDIR)/GUI_GetBitmapPixelColor.o \
	$(OBJDIR)/GUI_GetBitmapPixelIndex.o \
	$(OBJDIR)/GUI_GetClientRect.o \
	$(OBJDIR)/GUI_GetColor.o \
	$(OBJDIR)/GUI_GetDispPos.o \
	$(OBJDIR)/GUI_GetDrawMode.o \
	$(OBJDIR)/GUI_GetFont.o \
	$(OBJDIR)/GUI_GetFontInfo.o \
	$(OBJDIR)/GUI_GetFontSizeY.o \
	$(OBJDIR)/GUI_GetLineStyle.o \
	$(OBJDIR)/GUI_GetOrg.o \
	$(OBJDIR)/GUI_GetStringDistX.o \
	$(OBJDIR)/GUI_GetTextAlign.o \
	$(OBJDIR)/GUI_GetTextExtend.o \
	$(OBJDIR)/GUI_GetTextMode.o \
	$(OBJDIR)/GUI_GetVersionString.o \
	$(OBJDIR)/GUI_GetYSizeOfFont.o \
	$(OBJDIR)/GUI_GIF.o \
	$(OBJDIR)/GUI_Goto.o \
	$(OBJDIR)/GUI_InitLUT.o \
	$(OBJDIR)/GUI_InvertRect.o \
	$(OBJDIR)/GUI_IsInFont.o \
	$(OBJDIR)/GUI_Log.o \
	$(OBJDIR)/GUI_MergeRect.o \
	$(OBJDIR)/GUI_MOUSE.o \
	$(OBJDIR)/GUI_MOUSE_DriverPS2.o \
	$(OBJDIR)/GUI_MoveRect.o \
	$(OBJDIR)/GUI_OnKey.o \
	$(OBJDIR)/GUI_Pen.o \
	$(OBJDIR)/GUI_PID.o \
	$(OBJDIR)/GUI_RectsIntersect.o \
	$(OBJDIR)/GUI_SaveContext.o \
	$(OBJDIR)/GUI_ScreenSize.o \
	$(OBJDIR)/GUI_SelectLayer.o \
	$(OBJDIR)/GUI_SelectLCD.o \
	$(OBJDIR)/GUI_SetClipRect.o \
	$(OBJDIR)/GUI_SetColor.o \
	$(OBJDIR)/GUI_SetColorIndex.o \
	$(OBJDIR)/GUI_SetDecChar.o \
	$(OBJDIR)/GUI_SetDefault.o \
	$(OBJDIR)/GUI_SetDrawMode.o \
	$(OBJDIR)/GUI_SetFont.o \
	$(OBJDIR)/GUI_SetLBorder.o \
	$(OBJDIR)/GUI_SetLineStyle.o \
	$(OBJDIR)/GUI_SetLUTColor.o \
	$(OBJDIR)/GUI_SetLUTColorEx.o \
	$(OBJDIR)/GUI_SetLUTEntry.o \
	$(OBJDIR)/GUI_SetOrg.o \
	$(OBJDIR)/GUI_SetPixelIndex.o \
	$(OBJDIR)/GUI_SetTextAlign.o \
	$(OBJDIR)/GUI_SetTextMode.o \
	$(OBJDIR)/GUI_SetTextStyle.o \
	$(OBJDIR)/GUI_SIF.o \
	$(OBJDIR)/GUI_SIF_Prop.o \
	$(OBJDIR)/GUI_TOUCH.o \
	$(OBJDIR)/GUI_TOUCH_DriverAnalog.o \
	$(OBJDIR)/GUI_TOUCH_StoreState.o \
	$(OBJDIR)/GUI_TOUCH_StoreUnstable.o \
	$(OBJDIR)/GUI_UC.o \
	$(OBJDIR)/GUI_UC_ConvertUC2UTF8.o \
	$(OBJDIR)/GUI_UC_ConvertUTF82UC.o \
	$(OBJDIR)/GUI_UC_DispString.o \
	$(OBJDIR)/GUI_UC_EncodeNone.o \
	$(OBJDIR)/GUI_UC_EncodeUTF8.o \
	$(OBJDIR)/GUI_WaitEvent.o \
	$(OBJDIR)/GUI_WaitKey.o \
	$(OBJDIR)/GUI_Warn.o \
	$(OBJDIR)/GUIAlloc.o \
	$(OBJDIR)/GUIArc.o \
	$(OBJDIR)/GUIChar.o \
	$(OBJDIR)/GUICharLine.o \
	$(OBJDIR)/GUICharM.o \
	$(OBJDIR)/GUICharP.o \
	$(OBJDIR)/GUICirc.o \
	$(OBJDIR)/GUIColor2Index.o \
	$(OBJDIR)/GUICore.o \
	$(OBJDIR)/GUICurs.o \
	$(OBJDIR)/GUIEncJS.o \
	$(OBJDIR)/GUIIndex2Color.o \
	$(OBJDIR)/GUIPolyE.o \
	$(OBJDIR)/GUIPolyM.o \
	$(OBJDIR)/GUIPolyR.o \
	$(OBJDIR)/GUIRealloc.o \
	$(OBJDIR)/GUIStream.o \
	$(OBJDIR)/GUITask.o \
	$(OBJDIR)/GUITime.o \
	$(OBJDIR)/GUITimer.o \
	$(OBJDIR)/GUIUC0.o \
	$(OBJDIR)/GUIVal.o \
	$(OBJDIR)/GUIValf.o \
	$(OBJDIR)/LCD.o \
	$(OBJDIR)/LCD_API.o \
	$(OBJDIR)/LCD_DrawVLine.o \
	$(OBJDIR)/LCD_GetColorIndex.o \
	$(OBJDIR)/LCD_GetEx.o \
	$(OBJDIR)/LCD_GetNumDisplays.o \
	$(OBJDIR)/LCD_GetPixelColor.o \
	$(OBJDIR)/LCD_Index2ColorEx.o \
	$(OBJDIR)/LCD_L0_Generic.o \
	$(OBJDIR)/LCD_Mirror.o \
	$(OBJDIR)/LCD_MixColors256.o \
	$(OBJDIR)/LCD_ReadRect.o \
	$(OBJDIR)/LCD_Rotate180.o \
	$(OBJDIR)/LCD_RotateCCW.o \
	$(OBJDIR)/LCD_RotateCW.o \
	$(OBJDIR)/LCD_SelectLCD.o \
	$(OBJDIR)/LCD_SetAPI.o \
	$(OBJDIR)/LCD_SetClipRectEx.o \
	$(OBJDIR)/LCD_UpdateColorIndices.o \
	$(OBJDIR)/LCDAA.o \
	$(OBJDIR)/LCDColor.o \
	$(OBJDIR)/LCDGetP.o \
	$(OBJDIR)/LCDInfo0.o \
	$(OBJDIR)/LCDInfo1.o \
	$(OBJDIR)/LCDInfo.o \
	$(OBJDIR)/LCDL0Delta.o \
	$(OBJDIR)/LCDL0Mag.o \
	$(OBJDIR)/LCDP1.o \
	$(OBJDIR)/LCDP565_Index2Color.o \
	$(OBJDIR)/LCDP888_Index2Color.o \
	$(OBJDIR)/LCDPM565_Index2Color.o \
	$(OBJDIR)/LCDPM888_Index2Color.o \
	$(OBJDIR)/LCDRLE4.o \
	$(OBJDIR)/LCDRLE8.o \
	$(OBJDIR)/LCDRLE16.o

FONTOBJS = $(OBJDIR)/F4x6.o \
	$(OBJDIR)/F6x8.o \
	$(OBJDIR)/F08_1.o \
	$(OBJDIR)/F08_ASCII.o \
	$(OBJDIR)/F8x8.o \
	$(OBJDIR)/F8x10_ASCII.o \
	$(OBJDIR)/F8x12_ASCII.o \
	$(OBJDIR)/F8x13_1.o \
	$(OBJDIR)/F8x13_ASCII.o \
	$(OBJDIR)/F8x15B_1.o \
	$(OBJDIR)/F8x15B_ASCII.o \
	$(OBJDIR)/F8x16.o \
	$(OBJDIR)/F10_1.o \
	$(OBJDIR)/F10_ASCII.o \
	$(OBJDIR)/F10S_1.o \
	$(OBJDIR)/F10S_ASCII.o \
	$(OBJDIR)/F13_1.o \
	$(OBJDIR)/F13_ASCII.o \
	$(OBJDIR)/F13B_1.o \
	$(OBJDIR)/F13B_ASCII.o \
	$(OBJDIR)/F13H_1.o \
	$(OBJDIR)/F13H_ASCII.o \
	$(OBJDIR)/F13HB_1.o \
	$(OBJDIR)/F13HB_ASCII.o \
	$(OBJDIR)/F16_1.o \
	$(OBJDIR)/F16_1HK.o \
	$(OBJDIR)/F16_ASCII.o \
	$(OBJDIR)/F16_HK.o \
	$(OBJDIR)/F16B_1.o \
	$(OBJDIR)/F16B_ASCII.o \
	$(OBJDIR)/F24_1.o \
	$(OBJDIR)/F24_ASCII.o \
	$(OBJDIR)/F24B_1.o \
	$(OBJDIR)/F24B_ASCII.o \
	$(OBJDIR)/F32_1.o \
	$(OBJDIR)/F32_ASCII.o \
	$(OBJDIR)/F32B_1.o \
	$(OBJDIR)/F32B_ASCII.o \
	$(OBJDIR)/FComic18B_1.o \
	$(OBJDIR)/FComic18B_ASCII.o \
	$(OBJDIR)/FComic24B_1.o \
	$(OBJDIR)/FComic24B_ASCII.o \
	$(OBJDIR)/FD24x32.o \
	$(OBJDIR)/FD32.o \
	$(OBJDIR)/FD36x48.o \
	$(OBJDIR)/FD48.o \
	$(OBJDIR)/FD48x64.o \
	$(OBJDIR)/FD60x80.o \
	$(OBJDIR)/FD64.o \
	$(OBJDIR)/FD80.o
#	$(OBJDIR)/hzk12.o \
#	$(OBJDIR)/hzk16.o \
#	$(OBJDIR)/hzk24.o

MDOBJS = $(OBJDIR)/GUIDEV.o \
	$(OBJDIR)/GUIDEV_1.o \
	$(OBJDIR)/GUIDEV_8.o \
	$(OBJDIR)/GUIDEV_16.o \
	$(OBJDIR)/GUIDEV_AA.o \
	$(OBJDIR)/GUIDEV_Auto.o \
	$(OBJDIR)/GUIDEV_Banding.o \
	$(OBJDIR)/GUIDEV_Clear.o \
	$(OBJDIR)/GUIDEV_CmpWithLCD.o \
	$(OBJDIR)/GUIDEV_CopyFromLCD.o \
	$(OBJDIR)/GUIDEV_CreateFixed.o \
	$(OBJDIR)/GUIDEV_GetDataPtr.o \
	$(OBJDIR)/GUIDEV_GetXSize.o \
	$(OBJDIR)/GUIDEV_GetYSize.o \
	$(OBJDIR)/GUIDEV_Measure.o \
	$(OBJDIR)/GUIDEV_ReduceYSize.o \
	$(OBJDIR)/GUIDEV_SetOrg.o \
	$(OBJDIR)/GUIDEV_Usage.o \
	$(OBJDIR)/GUIDEV_UsageBM.o \
	$(OBJDIR)/GUIDEV_Write.o \
	$(OBJDIR)/GUIDEV_WriteAlpha.o \
	$(OBJDIR)/GUIDEV_WriteEx.o \
	$(OBJDIR)/GUIDEV_XY2PTR.o

WGOBJS = $(OBJDIR)/BUTTON.o \
	$(OBJDIR)/BUTTON__SetBitmapObj.o \
	$(OBJDIR)/BUTTON_Bitmap.o \
	$(OBJDIR)/BUTTON_BMP.o \
	$(OBJDIR)/BUTTON_Create.o \
	$(OBJDIR)/BUTTON_CreateIndirect.o \
	$(OBJDIR)/BUTTON_Default.o \
	$(OBJDIR)/BUTTON_Get.o \
	$(OBJDIR)/BUTTON_GetBitmap.o \
	$(OBJDIR)/BUTTON_IsPressed.o \
	$(OBJDIR)/BUTTON_SelfDraw.o \
	$(OBJDIR)/BUTTON_SetFocusColor.o \
	$(OBJDIR)/BUTTON_SetTextAlign.o \
	$(OBJDIR)/BUTTON_StreamedBitmap.o \
	$(OBJDIR)/CHECKBOX.o \
	$(OBJDIR)/CHECKBOX_Create.o \
	$(OBJDIR)/CHECKBOX_CreateIndirect.o \
	$(OBJDIR)/CHECKBOX_Default.o \
	$(OBJDIR)/CHECKBOX_GetState.o \
	$(OBJDIR)/CHECKBOX_GetText.o \
	$(OBJDIR)/CHECKBOX_Image.o \
	$(OBJDIR)/CHECKBOX_IsChecked.o \
	$(OBJDIR)/CHECKBOX_SetBkColor.o \
	$(OBJDIR)/CHECKBOX_SetBoxBkColor.o \
	$(OBJDIR)/CHECKBOX_SetDefaultImage.o \
	$(OBJDIR)/CHECKBOX_SetFocusColor.o \
	$(OBJDIR)/CHECKBOX_SetFont.o \
	$(OBJDIR)/CHECKBOX_SetImage.o \
	$(OBJDIR)/CHECKBOX_SetNumStates.o \
	$(OBJDIR)/CHECKBOX_SetSpacing.o \
	$(OBJDIR)/CHECKBOX_SetState.o \
	$(OBJDIR)/CHECKBOX_SetText.o \
	$(OBJDIR)/CHECKBOX_SetTextAlign.o \
	$(OBJDIR)/CHECKBOX_SetTextColor.o \
	$(OBJDIR)/DIALOG.o \
	$(OBJDIR)/DROPDOWN.o \
	$(OBJDIR)/DROPDOWN_AddString.o \
	$(OBJDIR)/DROPDOWN_Create.o \
	$(OBJDIR)/DROPDOWN_CreateIndirect.o \
	$(OBJDIR)/DROPDOWN_Default.o \
	$(OBJDIR)/DROPDOWN_DeleteItem.o \
	$(OBJDIR)/DROPDOWN_GetNumItems.o \
	$(OBJDIR)/DROPDOWN_InsertString.o \
	$(OBJDIR)/DROPDOWN_ItemSpacing.o \
	$(OBJDIR)/DROPDOWN_SetAutoScroll.o \
	$(OBJDIR)/DROPDOWN_SetBkColor.o \
	$(OBJDIR)/DROPDOWN_SetColor.o \
	$(OBJDIR)/DROPDOWN_SetFont.o \
	$(OBJDIR)/DROPDOWN_SetScrollbarColor.o \
	$(OBJDIR)/DROPDOWN_SetScrollbarWidth.o \
	$(OBJDIR)/DROPDOWN_SetTextAlign.o \
	$(OBJDIR)/DROPDOWN_SetTextColor.o \
	$(OBJDIR)/DROPDOWN_SetTextHeight.o \
	$(OBJDIR)/EDIT.o \
	$(OBJDIR)/EDIT_Create.o \
	$(OBJDIR)/EDIT_CreateIndirect.o \
	$(OBJDIR)/EDIT_Default.o \
	$(OBJDIR)/EDIT_GetCursorPixelPos.o \
	$(OBJDIR)/EDIT_GetCursorPos.o \
	$(OBJDIR)/EDIT_GetNumChars.o \
	$(OBJDIR)/EDIT_SetCursorAtChar.o \
	$(OBJDIR)/EDIT_SetFocussable.o \
	$(OBJDIR)/EDIT_SetInsertMode.o \
	$(OBJDIR)/EDIT_SetpfAddKeyEx.o \
	$(OBJDIR)/EDIT_SetpfUpdateBuffer.o \
	$(OBJDIR)/EDIT_SetSel.o \
	$(OBJDIR)/EDIT_SetTextMode.o \
	$(OBJDIR)/EDITBin.o \
	$(OBJDIR)/EDITDec.o \
	$(OBJDIR)/EDITFloat.o \
	$(OBJDIR)/EDITHex.o \
	$(OBJDIR)/EDITUlong.o \
	$(OBJDIR)/FRAMEWIN.o \
	$(OBJDIR)/FRAMEWIN__UpdateButtons.o \
	$(OBJDIR)/FRAMEWIN_AddMenu.o \
	$(OBJDIR)/FRAMEWIN_Button.o \
	$(OBJDIR)/FRAMEWIN_ButtonClose.o \
	$(OBJDIR)/FRAMEWIN_ButtonMax.o \
	$(OBJDIR)/FRAMEWIN_ButtonMin.o \
	$(OBJDIR)/FRAMEWIN_Create.o \
	$(OBJDIR)/FRAMEWIN_CreateIndirect.o \
	$(OBJDIR)/FRAMEWIN_Default.o \
	$(OBJDIR)/FRAMEWIN_Get.o \
	$(OBJDIR)/FRAMEWIN_IsMinMax.o \
	$(OBJDIR)/FRAMEWIN_MinMaxRest.o \
	$(OBJDIR)/FRAMEWIN_SetBorderSize.o \
	$(OBJDIR)/FRAMEWIN_SetColors.o \
	$(OBJDIR)/FRAMEWIN_SetFont.o \
	$(OBJDIR)/FRAMEWIN_SetResizeable.o \
	$(OBJDIR)/FRAMEWIN_SetTitleHeight.o \
	$(OBJDIR)/FRAMEWIN_SetTitleVis.o \
	$(OBJDIR)/GRAPH.o \
	$(OBJDIR)/GRAPH_CreateIndirect.o \
	$(OBJDIR)/GRAPH_DATA_XY.o \
	$(OBJDIR)/GRAPH_DATA_YT.o \
	$(OBJDIR)/GRAPH_SCALE.o \
	$(OBJDIR)/GUI_ARRAY.o \
	$(OBJDIR)/GUI_ARRAY_DeleteItem.o \
	$(OBJDIR)/GUI_ARRAY_InsertItem.o \
	$(OBJDIR)/GUI_ARRAY_ResizeItem.o \
	$(OBJDIR)/GUI_DRAW.o \
	$(OBJDIR)/GUI_DRAW_BITMAP.o \
	$(OBJDIR)/GUI_DRAW_BMP.o \
	$(OBJDIR)/GUI_DRAW_Self.o \
	$(OBJDIR)/GUI_DRAW_STREAMED.o \
	$(OBJDIR)/GUI_EditBin.o \
	$(OBJDIR)/GUI_EditDec.o \
	$(OBJDIR)/GUI_EditFloat.o \
	$(OBJDIR)/GUI_EditHex.o \
	$(OBJDIR)/GUI_EditString.o \
	$(OBJDIR)/GUI_HOOK.o \
	$(OBJDIR)/HEADER.o \
	$(OBJDIR)/HEADER__SetDrawObj.o \
	$(OBJDIR)/HEADER_Bitmap.o \
	$(OBJDIR)/HEADER_BMP.o \
	$(OBJDIR)/HEADER_Create.o \
	$(OBJDIR)/HEADER_CreateIndirect.o \
	$(OBJDIR)/HEADER_GetSel.o \
	$(OBJDIR)/HEADER_SetDragLimit.o \
	$(OBJDIR)/HEADER_StreamedBitmap.o \
	$(OBJDIR)/LISTBOX.o \
	$(OBJDIR)/LISTBOX_Create.o \
	$(OBJDIR)/LISTBOX_CreateIndirect.o \
	$(OBJDIR)/LISTBOX_Default.o \
	$(OBJDIR)/LISTBOX_DeleteItem.o \
	$(OBJDIR)/LISTBOX_Font.o \
	$(OBJDIR)/LISTBOX_GetItemText.o \
	$(OBJDIR)/LISTBOX_GetNumItems.o \
	$(OBJDIR)/LISTBOX_GetTextAlign.o \
	$(OBJDIR)/LISTBOX_InsertString.o \
	$(OBJDIR)/LISTBOX_ItemDisabled.o \
	$(OBJDIR)/LISTBOX_ItemSpacing.o \
	$(OBJDIR)/LISTBOX_MultiSel.o \
	$(OBJDIR)/LISTBOX_ScrollStep.o \
	$(OBJDIR)/LISTBOX_SetAutoScroll.o \
	$(OBJDIR)/LISTBOX_SetBkColor.o \
	$(OBJDIR)/LISTBOX_SetOwner.o \
	$(OBJDIR)/LISTBOX_SetOwnerDraw.o \
	$(OBJDIR)/LISTBOX_SetScrollbarColor.o \
	$(OBJDIR)/LISTBOX_SetScrollbarWidth.o \
	$(OBJDIR)/LISTBOX_SetString.o \
	$(OBJDIR)/LISTBOX_SetTextAlign.o \
	$(OBJDIR)/LISTBOX_SetTextColor.o \
	$(OBJDIR)/LISTVIEW.o \
	$(OBJDIR)/LISTVIEW_Create.o \
	$(OBJDIR)/LISTVIEW_CreateIndirect.o \
	$(OBJDIR)/LISTVIEW_Default.o \
	$(OBJDIR)/LISTVIEW_DeleteAllRows.o \
	$(OBJDIR)/LISTVIEW_DeleteColumn.o \
	$(OBJDIR)/LISTVIEW_DeleteRow.o \
	$(OBJDIR)/LISTVIEW_DisableRow.o \
	$(OBJDIR)/LISTVIEW_GetBkColor.o \
	$(OBJDIR)/LISTVIEW_GetFont.o \
	$(OBJDIR)/LISTVIEW_GetHeader.o \
	$(OBJDIR)/LISTVIEW_GetItemText.o \
	$(OBJDIR)/LISTVIEW_GetNumColumns.o \
	$(OBJDIR)/LISTVIEW_GetNumRows.o \
	$(OBJDIR)/LISTVIEW_GetSel.o \
	$(OBJDIR)/LISTVIEW_GetTextColor.o \
	$(OBJDIR)/LISTVIEW_InsertRow.o \
	$(OBJDIR)/LISTVIEW_SetAutoScroll.o \
	$(OBJDIR)/LISTVIEW_SetBkColor.o \
	$(OBJDIR)/LISTVIEW_SetColumnWidth.o \
	$(OBJDIR)/LISTVIEW_SetFont.o \
	$(OBJDIR)/LISTVIEW_SetGridVis.o \
	$(OBJDIR)/LISTVIEW_SetItemColor.o \
	$(OBJDIR)/LISTVIEW_SetItemText.o \
	$(OBJDIR)/LISTVIEW_SetLBorder.o \
	$(OBJDIR)/LISTVIEW_SetRBorder.o \
	$(OBJDIR)/LISTVIEW_SetRowHeight.o \
	$(OBJDIR)/LISTVIEW_SetSel.o \
	$(OBJDIR)/LISTVIEW_SetSort.o \
	$(OBJDIR)/LISTVIEW_SetTextAlign.o \
	$(OBJDIR)/LISTVIEW_SetTextColor.o \
	$(OBJDIR)/LISTVIEW_UserData.o \
	$(OBJDIR)/MENU.o \
	$(OBJDIR)/MENU__FindItem.o \
	$(OBJDIR)/MENU_Attach.o \
	$(OBJDIR)/MENU_CreateIndirect.o \
	$(OBJDIR)/MENU_Default.o \
	$(OBJDIR)/MENU_DeleteItem.o \
	$(OBJDIR)/MENU_DisableItem.o \
	$(OBJDIR)/MENU_EnableItem.o \
	$(OBJDIR)/MENU_GetItem.o \
	$(OBJDIR)/MENU_GetItemText.o \
	$(OBJDIR)/MENU_GetNumItems.o \
	$(OBJDIR)/MENU_InsertItem.o \
	$(OBJDIR)/MENU_Popup.o \
	$(OBJDIR)/MENU_SetBkColor.o \
	$(OBJDIR)/MENU_SetBorderSize.o \
	$(OBJDIR)/MENU_SetFont.o \
	$(OBJDIR)/MENU_SetItem.o \
	$(OBJDIR)/MENU_SetTextColor.o \
	$(OBJDIR)/MESSAGEBOX.o \
	$(OBJDIR)/MULTIEDIT.o \
	$(OBJDIR)/MULTIEDIT_Create.o \
	$(OBJDIR)/MULTIEDIT_CreateIndirect.o \
	$(OBJDIR)/MULTIPAGE.o \
	$(OBJDIR)/MULTIPAGE_Create.o \
	$(OBJDIR)/MULTIPAGE_CreateIndirect.o \
	$(OBJDIR)/MULTIPAGE_Default.o \
	$(OBJDIR)/PROGBAR.o \
	$(OBJDIR)/PROGBAR_Create.o \
	$(OBJDIR)/PROGBAR_CreateIndirect.o \
	$(OBJDIR)/PROGBAR_SetBarColor.o \
	$(OBJDIR)/PROGBAR_SetFont.o \
	$(OBJDIR)/PROGBAR_SetMinMax.o \
	$(OBJDIR)/PROGBAR_SetText.o \
	$(OBJDIR)/PROGBAR_SetTextAlign.o \
	$(OBJDIR)/PROGBAR_SetTextColor.o \
	$(OBJDIR)/PROGBAR_SetTextPos.o \
	$(OBJDIR)/RADIO.o \
	$(OBJDIR)/RADIO_Create.o \
	$(OBJDIR)/RADIO_CreateIndirect.o \
	$(OBJDIR)/RADIO_Default.o \
	$(OBJDIR)/RADIO_GetText.o \
	$(OBJDIR)/RADIO_Image.o \
	$(OBJDIR)/RADIO_SetBkColor.o \
	$(OBJDIR)/RADIO_SetDefaultImage.o \
	$(OBJDIR)/RADIO_SetFocusColor.o \
	$(OBJDIR)/RADIO_SetFont.o \
	$(OBJDIR)/RADIO_SetGroupId.o \
	$(OBJDIR)/RADIO_SetImage.o \
	$(OBJDIR)/RADIO_SetText.o \
	$(OBJDIR)/RADIO_SetTextColor.o \
	$(OBJDIR)/SCROLLBAR.o \
	$(OBJDIR)/SCROLLBAR_Create.o \
	$(OBJDIR)/SCROLLBAR_CreateIndirect.o \
	$(OBJDIR)/SCROLLBAR_Defaults.o \
	$(OBJDIR)/SCROLLBAR_GetValue.o \
	$(OBJDIR)/SCROLLBAR_SetColor.o \
	$(OBJDIR)/SCROLLBAR_SetWidth.o \
	$(OBJDIR)/SLIDER.o \
	$(OBJDIR)/SLIDER_Create.o \
	$(OBJDIR)/SLIDER_CreateIndirect.o \
	$(OBJDIR)/SLIDER_Default.o \
	$(OBJDIR)/SLIDER_SetFocusColor.o \
	$(OBJDIR)/TEXT.o \
	$(OBJDIR)/TEXT_Create.o \
	$(OBJDIR)/TEXT_CreateIndirect.o \
	$(OBJDIR)/TEXT_Default.o \
	$(OBJDIR)/TEXT_SetBkColor.o \
	$(OBJDIR)/TEXT_SetFont.o \
	$(OBJDIR)/TEXT_SetText.o \
	$(OBJDIR)/TEXT_SetTextAlign.o \
	$(OBJDIR)/TEXT_SetTextColor.o \
	$(OBJDIR)/TEXT_SetWrapMode.o \
	$(OBJDIR)/WIDGET.o \
	$(OBJDIR)/WIDGET_Effect_3D1L.o \
	$(OBJDIR)/WIDGET_Effect_3D2L.o \
	$(OBJDIR)/WIDGET_Effect_3D.o \
	$(OBJDIR)/WIDGET_Effect_None.o \
	$(OBJDIR)/WIDGET_Effect_Simple.o \
	$(OBJDIR)/WIDGET_FillStringInRect.o \
	$(OBJDIR)/WIDGET_SetEffect.o \
	$(OBJDIR)/WIDGET_SetWidth.o \
	$(OBJDIR)/WINDOW.o \
	$(OBJDIR)/WINDOW_Default.o

WMOBJS = $(OBJDIR)/WM.o \
	$(OBJDIR)/WM__ForEachDesc.o \
	$(OBJDIR)/WM__GetFirstSibling.o \
	$(OBJDIR)/WM__GetFocussedChild.o \
	$(OBJDIR)/WM__GetLastSibling.o \
	$(OBJDIR)/WM__GetOrg_AA.o \
	$(OBJDIR)/WM__GetPrevSibling.o \
	$(OBJDIR)/WM__IsAncestor.o \
	$(OBJDIR)/WM__IsChild.o \
	$(OBJDIR)/WM__IsEnabled.o \
	$(OBJDIR)/WM__NotifyVisChanged.o \
	$(OBJDIR)/WM__Screen2Client.o \
	$(OBJDIR)/WM__SendMessage.o \
	$(OBJDIR)/WM__SendMessageIfEnabled.o \
	$(OBJDIR)/WM__SendMessageNoPara.o \
	$(OBJDIR)/WM__UpdateChildPositions.o \
	$(OBJDIR)/WM_AttachWindow.o \
	$(OBJDIR)/WM_BringToBottom.o \
	$(OBJDIR)/WM_BringToTop.o \
	$(OBJDIR)/WM_Broadcast.o \
	$(OBJDIR)/WM_CheckScrollPos.o \
	$(OBJDIR)/WM_CriticalHandle.o \
	$(OBJDIR)/WM_DIAG.o \
	$(OBJDIR)/WM_EnableWindow.o \
	$(OBJDIR)/WM_ForEachDesc.o \
	$(OBJDIR)/WM_GetBkColor.o \
	$(OBJDIR)/WM_GetCallback.o \
	$(OBJDIR)/WM_GetClientRect.o \
	$(OBJDIR)/WM_GetClientWindow.o \
	$(OBJDIR)/WM_GetDesktopWindow.o \
	$(OBJDIR)/WM_GetDesktopWindowEx.o \
	$(OBJDIR)/WM_GetDiagInfo.o \
	$(OBJDIR)/WM_GetDialogItem.o \
	$(OBJDIR)/WM_GetFirstChild.o \
	$(OBJDIR)/WM_GetFlags.o \
	$(OBJDIR)/WM_GetFocussedWindow.o \
	$(OBJDIR)/WM_GetId.o \
	$(OBJDIR)/WM_GetInsideRect.o \
	$(OBJDIR)/WM_GetInsideRectExScrollbar.o \
	$(OBJDIR)/WM_GetInvalidRect.o \
	$(OBJDIR)/WM_GetNextSibling.o \
	$(OBJDIR)/WM_GetOrg.o \
	$(OBJDIR)/WM_GetParent.o \
	$(OBJDIR)/WM_GetPrevSibling.o \
	$(OBJDIR)/WM_GetScrollbar.o \
	$(OBJDIR)/WM_GetScrollPartner.o \
	$(OBJDIR)/WM_GetScrollPos.o \
	$(OBJDIR)/WM_GetScrollState.o \
	$(OBJDIR)/WM_GetWindowRect.o \
	$(OBJDIR)/WM_GetWindowSize.o \
	$(OBJDIR)/WM_HasCaptured.o \
	$(OBJDIR)/WM_HasFocus.o \
	$(OBJDIR)/WM_Hide.o \
	$(OBJDIR)/WM_InvalidateArea.o \
	$(OBJDIR)/WM_IsCompletelyVisible.o \
	$(OBJDIR)/WM_IsEnabled.o \
	$(OBJDIR)/WM_IsFocussable.o \
	$(OBJDIR)/WM_IsVisible.o \
	$(OBJDIR)/WM_IsWindow.o \
	$(OBJDIR)/WM_MakeModal.o \
	$(OBJDIR)/WM_Move.o \
	$(OBJDIR)/WM_MoveChildTo.o \
	$(OBJDIR)/WM_NotifyParent.o \
	$(OBJDIR)/WM_OnKey.o \
	$(OBJDIR)/WM_Paint.o \
	$(OBJDIR)/WM_PaintWindowAndDescs.o \
	$(OBJDIR)/WM_PID__GetPrevState.o \
	$(OBJDIR)/WM_ResizeWindow.o \
	$(OBJDIR)/WM_Screen2Win.o \
	$(OBJDIR)/WM_SendMessageNoPara.o \
	$(OBJDIR)/WM_SendToParent.o \
	$(OBJDIR)/WM_SetAnchor.o \
	$(OBJDIR)/WM_SetCallback.o \
	$(OBJDIR)/WM_SetCapture.o \
	$(OBJDIR)/WM_SetCaptureMove.o \
	$(OBJDIR)/WM_SetCreateFlags.o \
	$(OBJDIR)/WM_SetDesktopColor.o \
	$(OBJDIR)/WM_SetFocus.o \
	$(OBJDIR)/WM_SetFocusOnNextChild.o \
	$(OBJDIR)/WM_SetFocusOnPrevChild.o \
	$(OBJDIR)/WM_SetId.o \
	$(OBJDIR)/WM_SetpfPollPID.o \
	$(OBJDIR)/WM_SetScrollbar.o \
	$(OBJDIR)/WM_SetScrollPos.o \
	$(OBJDIR)/WM_SetScrollState.o \
	$(OBJDIR)/WM_SetSize.o \
	$(OBJDIR)/WM_SetTrans.o \
	$(OBJDIR)/WM_SetTransState.o \
	$(OBJDIR)/WM_SetUserClipRect.o \
	$(OBJDIR)/WM_SetWindowPos.o \
	$(OBJDIR)/WM_SetXSize.o \
	$(OBJDIR)/WM_SetYSize.o \
	$(OBJDIR)/WM_Show.o \
	$(OBJDIR)/WM_SIM.o \
	$(OBJDIR)/WM_StayOnTop.o \
	$(OBJDIR)/WM_Timer.o \
	$(OBJDIR)/WM_TimerExternal.o \
	$(OBJDIR)/WM_UpdateWindowAndDescs.o \
	$(OBJDIR)/WM_UserData.o \
	$(OBJDIR)/WM_Validate.o \
	$(OBJDIR)/WM_ValidateWindow.o \
	$(OBJDIR)/WMMemDev.o \
	$(OBJDIR)/WMTouch.o

#LCDOBJS = $(OBJDIR)/LCDWinFrameBuffer.o
LCDOBJS = $(OBJDIR)/LCDFrameBuffer.o
#LCDOBJS = $(OBJDIR)/LCDLinuxD2DFrameBuffer.o mapm.o

ifdef LINUX
GUIXOBJS = $(OBJDIR)/GUI_X_Linux.o
UCGUI = $(LIBDIR)/ucGUI_X.a
else
GUIXOBJS = $(OBJDIR)/GUI_X.o
UCGUI = $(LIBDIR)/ucGUI.a
endif

#UCGUI = $(LIBDIR)/ucGUI.a

all: $(UCGUI)

$(UCGUI): $(AAOBJS) $(CCOBJS) $(CMOBJS) $(COREOBJS) $(FONTOBJS) $(MDOBJS) $(WGOBJS) $(WMOBJS) $(LCDOBJS) $(GUIXOBJS)
	$(MKLIB) $(UCGUI) $(AAOBJS) $(CCOBJS) $(CMOBJS) $(COREOBJS) $(FONTOBJS) $(MDOBJS) $(WGOBJS) $(WMOBJS) $(LCDOBJS) $(GUIXOBJS) 2> log.txt

AADIR = GUI/AntiAlias
CCDIR = GUI/ConvertColor
CMDIR = GUI/ConvertMono
COREDIR = GUI/Core
FONTDIR = GUI/Font
MDDIR = GUI/MemDev
WGDIR = GUI/Widget
WMDIR = GUI/WM
LCDDIR = GUI/LCDDriver
GUIXDIR = GUI/GUI_X

$(AAOBJS): $(OBJDIR)/%.o: $(AADIR)/%.c
	$(CPP) $(CPPFLAG) -o $@ $<

$(CCOBJS): $(OBJDIR)/%.o: $(CCDIR)/%.c
	$(CPP) $(CPPFLAG) -o $@ $<

$(CMOBJS): $(OBJDIR)/%.o: $(CMDIR)/%.c
	$(CPP) $(CPPFLAG) -o $@ $<

$(COREOBJS): $(OBJDIR)/%.o: $(COREDIR)/%.c
	$(CPP) $(CPPFLAG) -o $@ $<

$(FONTOBJS): $(OBJDIR)/%.o: $(FONTDIR)/%.c
	$(CPP) $(CPPFLAG) -o $@ $<

$(MDOBJS): $(OBJDIR)/%.o: $(MDDIR)/%.c
	$(CPP) $(CPPFLAG) -o $@ $<

$(WGOBJS): $(OBJDIR)/%.o: $(WGDIR)/%.c
	$(CPP) $(CPPFLAG) -o $@ $<

$(WMOBJS): $(OBJDIR)/%.o: $(WMDIR)/%.c
	$(CPP) $(CPPFLAG) -o $@ $<

$(LCDOBJS): $(OBJDIR)/%.o: $(LCDDIR)/%.c
	$(CPP) $(CPPFLAG) -o $@ $<

$(GUIXOBJS): $(OBJDIR)/%.o: $(GUIXDIR)/%.c
	$(CPP) $(CPPFLAG) -o $@ $<

clean:
	rm $(AAOBJS) $(CCOBJS) $(CMOBJS) $(COREOBJS) $(FONTOBJS) $(MDOBJS) $(WGOBJS) $(WMOBJS) $(LCDOBJS) $(GUIXOBJS) $(UCGUI)

else

OUTDIR = OUTPUT
OBJDIR = $(OUTDIR)\OBJ
LIBDIR = $(OUTDIR)\LIB

CPP=cl.exe
#CP=gcc
MKLIB=link.exe -lib /nologo /out:
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "Config" /I "GUI\Core" /I "GUI\WM" /I "GUI\Widget" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fo"OUTPUT/OBJ/" /c
#CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "Config" /I "GUI\Core" /I "GUI\WM" /I "GUI\Widget" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fo"OUTPUT/OBJ/" /c 
UCGUI = $(LIBDIR)\unGUI.lib
ucGui1 = $(LIBDIR)\temp1.lib
ucGui2 = $(LIBDIR)\temp2.lib
ucGui3 = $(LIBDIR)\temp3.lib
ucGui4 = $(LIBDIR)\temp4.lib



AAOBJS = $(OBJDIR)\GUIAAArc.obj \
	$(OBJDIR)\GUIAAChar2.obj \
	$(OBJDIR)\GUIAAChar4.obj \
	$(OBJDIR)\GUIAAChar.obj \
	$(OBJDIR)\GUIAACircle.obj \
	$(OBJDIR)\GUIAALib.obj \
	$(OBJDIR)\GUIAALine.obj \
	$(OBJDIR)\GUIAAPoly.obj \
	$(OBJDIR)\GUIAAPolyOut.obj

CCOBJS = $(OBJDIR)\LCDP111.obj \
	$(OBJDIR)\LCDP222.obj \
	$(OBJDIR)\LCDP233.obj \
	$(OBJDIR)\LCDP323.obj \
	$(OBJDIR)\LCDP332.obj \
	$(OBJDIR)\LCDP444_12.obj \
	$(OBJDIR)\LCDP444_12_1.obj \
	$(OBJDIR)\LCDP444_16.obj \
	$(OBJDIR)\LCDP555.obj \
	$(OBJDIR)\LCDP556.obj \
	$(OBJDIR)\LCDP565.obj \
	$(OBJDIR)\LCDP655.obj \
	$(OBJDIR)\LCDP666.obj \
	$(OBJDIR)\LCDP888.obj \
	$(OBJDIR)\LCDP8666.obj \
	$(OBJDIR)\LCDP8666_1.obj \
	$(OBJDIR)\LCDPM233.obj \
	$(OBJDIR)\LCDPM323.obj \
	$(OBJDIR)\LCDPM332.obj \
	$(OBJDIR)\LCDPM444_12.obj \
	$(OBJDIR)\LCDPM444_16.obj \
	$(OBJDIR)\LCDPM555.obj \
	$(OBJDIR)\LCDPM556.obj \
	$(OBJDIR)\LCDPM565.obj \
	$(OBJDIR)\LCDPM655.obj \
	$(OBJDIR)\LCDPM666.obj \
	$(OBJDIR)\LCDPM888.obj

CMOBJS = $(OBJDIR)\LCDP0.obj \
	$(OBJDIR)\LCDP2.obj \
	$(OBJDIR)\LCDP4.obj

COREOBJS = $(OBJDIR)\GUI2DLib.obj \
	$(OBJDIR)\GUI__AddSpaceHex.obj \
	$(OBJDIR)\GUI__Arabic.obj \
	$(OBJDIR)\GUI__CalcTextRect.obj \
	$(OBJDIR)\GUI__DivideRound32.obj \
	$(OBJDIR)\GUI__DivideRound.obj \
	$(OBJDIR)\GUI__DrawBitmap16bpp.obj \
	$(OBJDIR)\GUI__GetCursorPos.obj \
	$(OBJDIR)\GUI__GetFontSizeY.obj \
	$(OBJDIR)\GUI__GetNumChars.obj \
	$(OBJDIR)\GUI__HandleEOLine.obj \
	$(OBJDIR)\GUI__IntersectRect.obj \
	$(OBJDIR)\GUI__IntersectRects.obj \
	$(OBJDIR)\GUI__memset16.obj \
	$(OBJDIR)\GUI__memset.obj \
	$(OBJDIR)\GUI__Read.obj \
	$(OBJDIR)\GUI__ReduceRect.obj \
	$(OBJDIR)\GUI__SetText.obj \
	$(OBJDIR)\GUI__strcmp.obj \
	$(OBJDIR)\GUI__strlen.obj \
	$(OBJDIR)\GUI__Wrap.obj \
	$(OBJDIR)\GUI_AddBin.obj \
	$(OBJDIR)\GUI_AddDec.obj \
	$(OBJDIR)\GUI_AddDecMin.obj \
	$(OBJDIR)\GUI_AddDecShift.obj \
	$(OBJDIR)\GUI_AddHex.obj \
	$(OBJDIR)\GUI_AddKeyMsgHook.obj \
	$(OBJDIR)\GUI_ALLOC_AllocInit.obj \
	$(OBJDIR)\GUI_ALLOC_AllocZero.obj \
	$(OBJDIR)\GUI_BMP.obj \
	$(OBJDIR)\GUI_BMP_Serialize.obj \
	$(OBJDIR)\GUI_CalcColorDist.obj \
	$(OBJDIR)\GUI_ClearRectEx.obj \
	$(OBJDIR)\GUI_Color2VisColor.obj \
	$(OBJDIR)\GUI_CursorArrowL.obj \
	$(OBJDIR)\GUI_CursorArrowLI.obj \
	$(OBJDIR)\GUI_CursorArrowLPx.obj \
	$(OBJDIR)\GUI_CursorArrowM.obj \
	$(OBJDIR)\GUI_CursorArrowMI.obj \
	$(OBJDIR)\GUI_CursorArrowMPx.obj \
	$(OBJDIR)\GUI_CursorArrowS.obj \
	$(OBJDIR)\GUI_CursorArrowSI.obj \
	$(OBJDIR)\GUI_CursorArrowSPx.obj \
	$(OBJDIR)\GUI_CursorCrossL.obj \
	$(OBJDIR)\GUI_CursorCrossLI.obj \
	$(OBJDIR)\GUI_CursorCrossLPx.obj \
	$(OBJDIR)\GUI_CursorCrossM.obj \
	$(OBJDIR)\GUI_CursorCrossMI.obj \
	$(OBJDIR)\GUI_CursorCrossMPx.obj \
	$(OBJDIR)\GUI_CursorCrossS.obj \
	$(OBJDIR)\GUI_CursorCrossSI.obj \
	$(OBJDIR)\GUI_CursorCrossSPx.obj \
	$(OBJDIR)\GUI_CursorHeaderM.obj \
	$(OBJDIR)\GUI_CursorHeaderMI.obj \
	$(OBJDIR)\GUI_CursorHeaderMPx.obj \
	$(OBJDIR)\GUI_CursorPal.obj \
	$(OBJDIR)\GUI_CursorPalI.obj \
	$(OBJDIR)\GUI_DispBin.obj \
	$(OBJDIR)\GUI_DispCEOL.obj \
	$(OBJDIR)\GUI_DispChar.obj \
	$(OBJDIR)\GUI_DispChars.obj \
	$(OBJDIR)\GUI_DispHex.obj \
	$(OBJDIR)\GUI_DispString.obj \
	$(OBJDIR)\GUI_DispStringAt.obj \
	$(OBJDIR)\GUI_DispStringAtCEOL.obj \
	$(OBJDIR)\GUI_DispStringHCenter.obj \
	$(OBJDIR)\GUI_DispStringInRect.obj \
	$(OBJDIR)\GUI_DispStringInRectEx.obj \
	$(OBJDIR)\GUI_DispStringInRectWrap.obj \
	$(OBJDIR)\GUI_DispStringLen.obj \
	$(OBJDIR)\GUI_DrawBitmap.obj \
	$(OBJDIR)\GUI_DrawBitmap_555.obj \
	$(OBJDIR)\GUI_DrawBitmap_565.obj \
	$(OBJDIR)\GUI_DrawBitmap_888.obj \
	$(OBJDIR)\GUI_DrawBitmapEx.obj \
	$(OBJDIR)\GUI_DrawBitmapExp.obj \
	$(OBJDIR)\GUI_DrawBitmapMag.obj \
	$(OBJDIR)\GUI_DrawFocusRect.obj \
	$(OBJDIR)\GUI_DrawGraph.obj \
	$(OBJDIR)\GUI_DrawGraphEx.obj \
	$(OBJDIR)\GUI_DrawHLine.obj \
	$(OBJDIR)\GUI_DrawLine.obj \
	$(OBJDIR)\GUI_DrawPie.obj \
	$(OBJDIR)\GUI_DrawPixel.obj \
	$(OBJDIR)\GUI_DrawPoint.obj \
	$(OBJDIR)\GUI_DrawPolygon.obj \
	$(OBJDIR)\GUI_DrawPolyline.obj \
	$(OBJDIR)\GUI_DrawRectEx.obj \
	$(OBJDIR)\GUI_DrawVLine.obj \
	$(OBJDIR)\GUI_ErrorOut.obj \
	$(OBJDIR)\GUI_Exec.obj \
	$(OBJDIR)\GUI_FillPolygon.obj \
	$(OBJDIR)\GUI_FillRect.obj \
	$(OBJDIR)\GUI_FillRectEx.obj \
	$(OBJDIR)\GUI_GetBitmapPixelColor.obj \
	$(OBJDIR)\GUI_GetBitmapPixelIndex.obj \
	$(OBJDIR)\GUI_GetClientRect.obj \
	$(OBJDIR)\GUI_GetColor.obj \
	$(OBJDIR)\GUI_GetDispPos.obj \
	$(OBJDIR)\GUI_GetDrawMode.obj \
	$(OBJDIR)\GUI_GetFont.obj \
	$(OBJDIR)\GUI_GetFontInfo.obj \
	$(OBJDIR)\GUI_GetFontSizeY.obj \
	$(OBJDIR)\GUI_GetLineStyle.obj \
	$(OBJDIR)\GUI_GetOrg.obj \
	$(OBJDIR)\GUI_GetStringDistX.obj \
	$(OBJDIR)\GUI_GetTextAlign.obj \
	$(OBJDIR)\GUI_GetTextExtend.obj \
	$(OBJDIR)\GUI_GetTextMode.obj \
	$(OBJDIR)\GUI_GetVersionString.obj \
	$(OBJDIR)\GUI_GetYSizeOfFont.obj \
	$(OBJDIR)\GUI_GIF.obj \
	$(OBJDIR)\GUI_Goto.obj \
	$(OBJDIR)\GUI_InitLUT.obj \
	$(OBJDIR)\GUI_InvertRect.obj \
	$(OBJDIR)\GUI_IsInFont.obj \
	$(OBJDIR)\GUI_Log.obj \
	$(OBJDIR)\GUI_MergeRect.obj \
	$(OBJDIR)\GUI_MOUSE.obj \
	$(OBJDIR)\GUI_MOUSE_DriverPS2.obj \
	$(OBJDIR)\GUI_MoveRect.obj \
	$(OBJDIR)\GUI_OnKey.obj \
	$(OBJDIR)\GUI_Pen.obj \
	$(OBJDIR)\GUI_PID.obj \
	$(OBJDIR)\GUI_RectsIntersect.obj \
	$(OBJDIR)\GUI_SaveContext.obj \
	$(OBJDIR)\GUI_ScreenSize.obj \
	$(OBJDIR)\GUI_SelectLayer.obj \
	$(OBJDIR)\GUI_SelectLCD.obj \
	$(OBJDIR)\GUI_SetClipRect.obj \
	$(OBJDIR)\GUI_SetColor.obj \
	$(OBJDIR)\GUI_SetColorIndex.obj \
	$(OBJDIR)\GUI_SetDecChar.obj \
	$(OBJDIR)\GUI_SetDefault.obj \
	$(OBJDIR)\GUI_SetDrawMode.obj \
	$(OBJDIR)\GUI_SetFont.obj \
	$(OBJDIR)\GUI_SetLBorder.obj \
	$(OBJDIR)\GUI_SetLineStyle.obj \
	$(OBJDIR)\GUI_SetLUTColor.obj \
	$(OBJDIR)\GUI_SetLUTColorEx.obj \
	$(OBJDIR)\GUI_SetLUTEntry.obj \
	$(OBJDIR)\GUI_SetOrg.obj \
	$(OBJDIR)\GUI_SetPixelIndex.obj \
	$(OBJDIR)\GUI_SetTextAlign.obj \
	$(OBJDIR)\GUI_SetTextMode.obj \
	$(OBJDIR)\GUI_SetTextStyle.obj \
	$(OBJDIR)\GUI_SIF.obj \
	$(OBJDIR)\GUI_SIF_Prop.obj \
	$(OBJDIR)\GUI_TOUCH.obj \
	$(OBJDIR)\GUI_TOUCH_DriverAnalog.obj \
	$(OBJDIR)\GUI_TOUCH_StoreState.obj \
	$(OBJDIR)\GUI_TOUCH_StoreUnstable.obj \
	$(OBJDIR)\GUI_UC.obj \
	$(OBJDIR)\GUI_UC_ConvertUC2UTF8.obj \
	$(OBJDIR)\GUI_UC_ConvertUTF82UC.obj \
	$(OBJDIR)\GUI_UC_DispString.obj \
	$(OBJDIR)\GUI_UC_EncodeNone.obj \
	$(OBJDIR)\GUI_UC_EncodeUTF8.obj \
	$(OBJDIR)\GUI_WaitEvent.obj \
	$(OBJDIR)\GUI_WaitKey.obj \
	$(OBJDIR)\GUI_Warn.obj \
	$(OBJDIR)\GUIAlloc.obj \
	$(OBJDIR)\GUIArc.obj \
	$(OBJDIR)\GUIChar.obj \
	$(OBJDIR)\GUICharLine.obj \
	$(OBJDIR)\GUICharM.obj \
	$(OBJDIR)\GUICharP.obj \
	$(OBJDIR)\GUICirc.obj \
	$(OBJDIR)\GUIColor2Index.obj \
	$(OBJDIR)\GUICore.obj \
	$(OBJDIR)\GUICurs.obj \
	$(OBJDIR)\GUIEncJS.obj \
	$(OBJDIR)\GUIIndex2Color.obj \
	$(OBJDIR)\GUIPolyE.obj \
	$(OBJDIR)\GUIPolyM.obj \
	$(OBJDIR)\GUIPolyR.obj \
	$(OBJDIR)\GUIRealloc.obj \
	$(OBJDIR)\GUIStream.obj \
	$(OBJDIR)\GUITask.obj \
	$(OBJDIR)\GUITime.obj \
	$(OBJDIR)\GUITimer.obj \
	$(OBJDIR)\GUIUC0.obj \
	$(OBJDIR)\GUIVal.obj \
	$(OBJDIR)\GUIValf.obj \
	$(OBJDIR)\LCD.obj \
	$(OBJDIR)\LCD_API.obj \
	$(OBJDIR)\LCD_DrawVLine.obj \
	$(OBJDIR)\LCD_GetColorIndex.obj \
	$(OBJDIR)\LCD_GetEx.obj \
	$(OBJDIR)\LCD_GetNumDisplays.obj \
	$(OBJDIR)\LCD_GetPixelColor.obj \
	$(OBJDIR)\LCD_Index2ColorEx.obj \
	$(OBJDIR)\LCD_L0_Generic.obj \
	$(OBJDIR)\LCD_Mirror.obj \
	$(OBJDIR)\LCD_MixColors256.obj \
	$(OBJDIR)\LCD_ReadRect.obj \
	$(OBJDIR)\LCD_Rotate180.obj \
	$(OBJDIR)\LCD_RotateCCW.obj \
	$(OBJDIR)\LCD_RotateCW.obj \
	$(OBJDIR)\LCD_SelectLCD.obj \
	$(OBJDIR)\LCD_SetAPI.obj \
	$(OBJDIR)\LCD_SetClipRectEx.obj \
	$(OBJDIR)\LCD_UpdateColorIndices.obj \
	$(OBJDIR)\LCDAA.obj \
	$(OBJDIR)\LCDColor.obj \
	$(OBJDIR)\LCDGetP.obj \
	$(OBJDIR)\LCDInfo0.obj \
	$(OBJDIR)\LCDInfo1.obj \
	$(OBJDIR)\LCDInfo.obj \
	$(OBJDIR)\LCDL0Delta.obj \
	$(OBJDIR)\LCDL0Mag.obj \
	$(OBJDIR)\LCDP1.obj \
	$(OBJDIR)\LCDP565_Index2Color.obj \
	$(OBJDIR)\LCDP888_Index2Color.obj \
	$(OBJDIR)\LCDPM565_Index2Color.obj \
	$(OBJDIR)\LCDPM888_Index2Color.obj \
	$(OBJDIR)\LCDRLE4.obj \
	$(OBJDIR)\LCDRLE8.obj \
	$(OBJDIR)\LCDRLE16.obj

FONTOBJS = $(OBJDIR)\F4x6.obj \
	$(OBJDIR)\F6x8.obj \
	$(OBJDIR)\F08_1.obj \
	$(OBJDIR)\F08_ASCII.obj \
	$(OBJDIR)\F8x8.obj \
	$(OBJDIR)\F8x10_ASCII.obj \
	$(OBJDIR)\F8x12_ASCII.obj \
	$(OBJDIR)\F8x13_1.obj \
	$(OBJDIR)\F8x13_ASCII.obj \
	$(OBJDIR)\F8x15B_1.obj \
	$(OBJDIR)\F8x15B_ASCII.obj \
	$(OBJDIR)\F8x16.obj \
	$(OBJDIR)\F10_1.obj \
	$(OBJDIR)\F10_ASCII.obj \
	$(OBJDIR)\F10S_1.obj \
	$(OBJDIR)\F10S_ASCII.obj \
	$(OBJDIR)\F13_1.obj \
	$(OBJDIR)\F13_ASCII.obj \
	$(OBJDIR)\F13B_1.obj \
	$(OBJDIR)\F13B_ASCII.obj \
	$(OBJDIR)\F13H_1.obj \
	$(OBJDIR)\F13H_ASCII.obj \
	$(OBJDIR)\F13HB_1.obj \
	$(OBJDIR)\F13HB_ASCII.obj \
	$(OBJDIR)\F16_1.obj \
	$(OBJDIR)\F16_1HK.obj \
	$(OBJDIR)\F16_ASCII.obj \
	$(OBJDIR)\F16_HK.obj \
	$(OBJDIR)\F16B_1.obj \
	$(OBJDIR)\F16B_ASCII.obj \
	$(OBJDIR)\F24_1.obj \
	$(OBJDIR)\F24_ASCII.obj \
	$(OBJDIR)\F24B_1.obj \
	$(OBJDIR)\F24B_ASCII.obj \
	$(OBJDIR)\F32_1.obj \
	$(OBJDIR)\F32_ASCII.obj \
	$(OBJDIR)\F32B_1.obj \
	$(OBJDIR)\F32B_ASCII.obj \
	$(OBJDIR)\FComic18B_1.obj \
	$(OBJDIR)\FComic18B_ASCII.obj \
	$(OBJDIR)\FComic24B_1.obj \
	$(OBJDIR)\FComic24B_ASCII.obj \
	$(OBJDIR)\FD24x32.obj \
	$(OBJDIR)\FD32.obj \
	$(OBJDIR)\FD36x48.obj \
	$(OBJDIR)\FD48.obj \
	$(OBJDIR)\FD48x64.obj \
	$(OBJDIR)\FD60x80.obj \
	$(OBJDIR)\FD64.obj \
	$(OBJDIR)\FD80.obj
#	$(OBJDIR)\hzk12.obj \
#	$(OBJDIR)\hzk16.obj \
#	$(OBJDIR)\hzk24.obj

MDOBJS = $(OBJDIR)\GUIDEV.obj \
	$(OBJDIR)\GUIDEV_1.obj \
	$(OBJDIR)\GUIDEV_8.obj \
	$(OBJDIR)\GUIDEV_16.obj \
	$(OBJDIR)\GUIDEV_AA.obj \
	$(OBJDIR)\GUIDEV_Auto.obj \
	$(OBJDIR)\GUIDEV_Banding.obj \
	$(OBJDIR)\GUIDEV_Clear.obj \
	$(OBJDIR)\GUIDEV_CmpWithLCD.obj \
	$(OBJDIR)\GUIDEV_CopyFromLCD.obj \
	$(OBJDIR)\GUIDEV_CreateFixed.obj \
	$(OBJDIR)\GUIDEV_GetDataPtr.obj \
	$(OBJDIR)\GUIDEV_GetXSize.obj \
	$(OBJDIR)\GUIDEV_GetYSize.obj \
	$(OBJDIR)\GUIDEV_Measure.obj \
	$(OBJDIR)\GUIDEV_ReduceYSize.obj \
	$(OBJDIR)\GUIDEV_SetOrg.obj \
	$(OBJDIR)\GUIDEV_Usage.obj \
	$(OBJDIR)\GUIDEV_UsageBM.obj \
	$(OBJDIR)\GUIDEV_Write.obj \
	$(OBJDIR)\GUIDEV_WriteAlpha.obj \
	$(OBJDIR)\GUIDEV_WriteEx.obj \
	$(OBJDIR)\GUIDEV_XY2PTR.obj

WGOBJS = $(OBJDIR)\BUTTON.obj \
	$(OBJDIR)\BUTTON__SetBitmapObj.obj \
	$(OBJDIR)\BUTTON_Bitmap.obj \
	$(OBJDIR)\BUTTON_BMP.obj \
	$(OBJDIR)\BUTTON_Create.obj \
	$(OBJDIR)\BUTTON_CreateIndirect.obj \
	$(OBJDIR)\BUTTON_Default.obj \
	$(OBJDIR)\BUTTON_Get.obj \
	$(OBJDIR)\BUTTON_GetBitmap.obj \
	$(OBJDIR)\BUTTON_IsPressed.obj \
	$(OBJDIR)\BUTTON_SelfDraw.obj \
	$(OBJDIR)\BUTTON_SetFocusColor.obj \
	$(OBJDIR)\BUTTON_SetTextAlign.obj \
	$(OBJDIR)\BUTTON_StreamedBitmap.obj \
	$(OBJDIR)\CHECKBOX.obj \
	$(OBJDIR)\CHECKBOX_Create.obj \
	$(OBJDIR)\CHECKBOX_CreateIndirect.obj \
	$(OBJDIR)\CHECKBOX_Default.obj \
	$(OBJDIR)\CHECKBOX_GetState.obj \
	$(OBJDIR)\CHECKBOX_GetText.obj \
	$(OBJDIR)\CHECKBOX_Image.obj \
	$(OBJDIR)\CHECKBOX_IsChecked.obj \
	$(OBJDIR)\CHECKBOX_SetBkColor.obj \
	$(OBJDIR)\CHECKBOX_SetBoxBkColor.obj \
	$(OBJDIR)\CHECKBOX_SetDefaultImage.obj \
	$(OBJDIR)\CHECKBOX_SetFocusColor.obj \
	$(OBJDIR)\CHECKBOX_SetFont.obj \
	$(OBJDIR)\CHECKBOX_SetImage.obj \
	$(OBJDIR)\CHECKBOX_SetNumStates.obj \
	$(OBJDIR)\CHECKBOX_SetSpacing.obj \
	$(OBJDIR)\CHECKBOX_SetState.obj \
	$(OBJDIR)\CHECKBOX_SetText.obj \
	$(OBJDIR)\CHECKBOX_SetTextAlign.obj \
	$(OBJDIR)\CHECKBOX_SetTextColor.obj \
	$(OBJDIR)\DIALOG.obj \
	$(OBJDIR)\DROPDOWN.obj \
	$(OBJDIR)\DROPDOWN_AddString.obj \
	$(OBJDIR)\DROPDOWN_Create.obj \
	$(OBJDIR)\DROPDOWN_CreateIndirect.obj \
	$(OBJDIR)\DROPDOWN_Default.obj \
	$(OBJDIR)\DROPDOWN_DeleteItem.obj \
	$(OBJDIR)\DROPDOWN_GetNumItems.obj \
	$(OBJDIR)\DROPDOWN_InsertString.obj \
	$(OBJDIR)\DROPDOWN_ItemSpacing.obj \
	$(OBJDIR)\DROPDOWN_SetAutoScroll.obj \
	$(OBJDIR)\DROPDOWN_SetBkColor.obj \
	$(OBJDIR)\DROPDOWN_SetColor.obj \
	$(OBJDIR)\DROPDOWN_SetFont.obj \
	$(OBJDIR)\DROPDOWN_SetScrollbarColor.obj \
	$(OBJDIR)\DROPDOWN_SetScrollbarWidth.obj \
	$(OBJDIR)\DROPDOWN_SetTextAlign.obj \
	$(OBJDIR)\DROPDOWN_SetTextColor.obj \
	$(OBJDIR)\DROPDOWN_SetTextHeight.obj \
	$(OBJDIR)\EDIT.obj \
	$(OBJDIR)\EDIT_Create.obj \
	$(OBJDIR)\EDIT_CreateIndirect.obj \
	$(OBJDIR)\EDIT_Default.obj \
	$(OBJDIR)\EDIT_GetCursorPixelPos.obj \
	$(OBJDIR)\EDIT_GetCursorPos.obj \
	$(OBJDIR)\EDIT_GetNumChars.obj \
	$(OBJDIR)\EDIT_SetCursorAtChar.obj \
	$(OBJDIR)\EDIT_SetFocussable.obj \
	$(OBJDIR)\EDIT_SetInsertMode.obj \
	$(OBJDIR)\EDIT_SetpfAddKeyEx.obj \
	$(OBJDIR)\EDIT_SetpfUpdateBuffer.obj \
	$(OBJDIR)\EDIT_SetSel.obj \
	$(OBJDIR)\EDIT_SetTextMode.obj \
	$(OBJDIR)\EDITBin.obj \
	$(OBJDIR)\EDITDec.obj \
	$(OBJDIR)\EDITFloat.obj \
	$(OBJDIR)\EDITHex.obj \
	$(OBJDIR)\EDITUlong.obj \
	$(OBJDIR)\FRAMEWIN.obj \
	$(OBJDIR)\FRAMEWIN__UpdateButtons.obj \
	$(OBJDIR)\FRAMEWIN_AddMenu.obj \
	$(OBJDIR)\FRAMEWIN_Button.obj \
	$(OBJDIR)\FRAMEWIN_ButtonClose.obj \
	$(OBJDIR)\FRAMEWIN_ButtonMax.obj \
	$(OBJDIR)\FRAMEWIN_ButtonMin.obj \
	$(OBJDIR)\FRAMEWIN_Create.obj \
	$(OBJDIR)\FRAMEWIN_CreateIndirect.obj \
	$(OBJDIR)\FRAMEWIN_Default.obj \
	$(OBJDIR)\FRAMEWIN_Get.obj \
	$(OBJDIR)\FRAMEWIN_IsMinMax.obj \
	$(OBJDIR)\FRAMEWIN_MinMaxRest.obj \
	$(OBJDIR)\FRAMEWIN_SetBorderSize.obj \
	$(OBJDIR)\FRAMEWIN_SetColors.obj \
	$(OBJDIR)\FRAMEWIN_SetFont.obj \
	$(OBJDIR)\FRAMEWIN_SetResizeable.obj \
	$(OBJDIR)\FRAMEWIN_SetTitleHeight.obj \
	$(OBJDIR)\FRAMEWIN_SetTitleVis.obj \
	$(OBJDIR)\GRAPH.obj \
	$(OBJDIR)\GRAPH_CreateIndirect.obj \
	$(OBJDIR)\GRAPH_DATA_XY.obj \
	$(OBJDIR)\GRAPH_DATA_YT.obj \
	$(OBJDIR)\GRAPH_SCALE.obj \
	$(OBJDIR)\GUI_ARRAY.obj \
	$(OBJDIR)\GUI_ARRAY_DeleteItem.obj \
	$(OBJDIR)\GUI_ARRAY_InsertItem.obj \
	$(OBJDIR)\GUI_ARRAY_ResizeItem.obj \
	$(OBJDIR)\GUI_DRAW.obj \
	$(OBJDIR)\GUI_DRAW_BITMAP.obj \
	$(OBJDIR)\GUI_DRAW_BMP.obj \
	$(OBJDIR)\GUI_DRAW_Self.obj \
	$(OBJDIR)\GUI_DRAW_STREAMED.obj \
	$(OBJDIR)\GUI_EditBin.obj \
	$(OBJDIR)\GUI_EditDec.obj \
	$(OBJDIR)\GUI_EditFloat.obj \
	$(OBJDIR)\GUI_EditHex.obj \
	$(OBJDIR)\GUI_EditString.obj \
	$(OBJDIR)\GUI_HOOK.obj \
	$(OBJDIR)\HEADER.obj \
	$(OBJDIR)\HEADER__SetDrawObj.obj \
	$(OBJDIR)\HEADER_Bitmap.obj \
	$(OBJDIR)\HEADER_BMP.obj \
	$(OBJDIR)\HEADER_Create.obj \
	$(OBJDIR)\HEADER_CreateIndirect.obj \
	$(OBJDIR)\HEADER_GetSel.obj \
	$(OBJDIR)\HEADER_SetDragLimit.obj \
	$(OBJDIR)\HEADER_StreamedBitmap.obj

WGOBJS2 = $(OBJDIR)\LISTBOX.obj \
	$(OBJDIR)\LISTBOX_Create.obj \
	$(OBJDIR)\LISTBOX_CreateIndirect.obj \
	$(OBJDIR)\LISTBOX_Default.obj \
	$(OBJDIR)\LISTBOX_DeleteItem.obj \
	$(OBJDIR)\LISTBOX_Font.obj \
	$(OBJDIR)\LISTBOX_GetItemText.obj \
	$(OBJDIR)\LISTBOX_GetNumItems.obj \
	$(OBJDIR)\LISTBOX_GetTextAlign.obj \
	$(OBJDIR)\LISTBOX_InsertString.obj \
	$(OBJDIR)\LISTBOX_ItemDisabled.obj \
	$(OBJDIR)\LISTBOX_ItemSpacing.obj \
	$(OBJDIR)\LISTBOX_MultiSel.obj \
	$(OBJDIR)\LISTBOX_ScrollStep.obj \
	$(OBJDIR)\LISTBOX_SetAutoScroll.obj \
	$(OBJDIR)\LISTBOX_SetBkColor.obj \
	$(OBJDIR)\LISTBOX_SetOwner.obj \
	$(OBJDIR)\LISTBOX_SetOwnerDraw.obj \
	$(OBJDIR)\LISTBOX_SetScrollbarColor.obj \
	$(OBJDIR)\LISTBOX_SetScrollbarWidth.obj \
	$(OBJDIR)\LISTBOX_SetString.obj \
	$(OBJDIR)\LISTBOX_SetTextAlign.obj \
	$(OBJDIR)\LISTBOX_SetTextColor.obj \
	$(OBJDIR)\LISTVIEW.obj \
	$(OBJDIR)\LISTVIEW_Create.obj \
	$(OBJDIR)\LISTVIEW_CreateIndirect.obj \
	$(OBJDIR)\LISTVIEW_Default.obj \
	$(OBJDIR)\LISTVIEW_DeleteAllRows.obj \
	$(OBJDIR)\LISTVIEW_DeleteColumn.obj \
	$(OBJDIR)\LISTVIEW_DeleteRow.obj \
	$(OBJDIR)\LISTVIEW_DisableRow.obj \
	$(OBJDIR)\LISTVIEW_GetBkColor.obj \
	$(OBJDIR)\LISTVIEW_GetFont.obj \
	$(OBJDIR)\LISTVIEW_GetHeader.obj \
	$(OBJDIR)\LISTVIEW_GetItemText.obj \
	$(OBJDIR)\LISTVIEW_GetNumColumns.obj \
	$(OBJDIR)\LISTVIEW_GetNumRows.obj \
	$(OBJDIR)\LISTVIEW_GetSel.obj \
	$(OBJDIR)\LISTVIEW_GetTextColor.obj \
	$(OBJDIR)\LISTVIEW_InsertRow.obj \
	$(OBJDIR)\LISTVIEW_SetAutoScroll.obj \
	$(OBJDIR)\LISTVIEW_SetBkColor.obj \
	$(OBJDIR)\LISTVIEW_SetColumnWidth.obj \
	$(OBJDIR)\LISTVIEW_SetFont.obj \
	$(OBJDIR)\LISTVIEW_SetGridVis.obj \
	$(OBJDIR)\LISTVIEW_SetItemColor.obj \
	$(OBJDIR)\LISTVIEW_SetItemText.obj \
	$(OBJDIR)\LISTVIEW_SetLBorder.obj \
	$(OBJDIR)\LISTVIEW_SetRBorder.obj \
	$(OBJDIR)\LISTVIEW_SetRowHeight.obj \
	$(OBJDIR)\LISTVIEW_SetSel.obj \
	$(OBJDIR)\LISTVIEW_SetSort.obj \
	$(OBJDIR)\LISTVIEW_SetTextAlign.obj \
	$(OBJDIR)\LISTVIEW_SetTextColor.obj \
	$(OBJDIR)\LISTVIEW_UserData.obj \
	$(OBJDIR)\MENU.obj \
	$(OBJDIR)\MENU__FindItem.obj \
	$(OBJDIR)\MENU_Attach.obj \
	$(OBJDIR)\MENU_CreateIndirect.obj \
	$(OBJDIR)\MENU_Default.obj \
	$(OBJDIR)\MENU_DeleteItem.obj \
	$(OBJDIR)\MENU_DisableItem.obj \
	$(OBJDIR)\MENU_EnableItem.obj \
	$(OBJDIR)\MENU_GetItem.obj \
	$(OBJDIR)\MENU_GetItemText.obj \
	$(OBJDIR)\MENU_GetNumItems.obj \
	$(OBJDIR)\MENU_InsertItem.obj \
	$(OBJDIR)\MENU_Popup.obj \
	$(OBJDIR)\MENU_SetBkColor.obj \
	$(OBJDIR)\MENU_SetBorderSize.obj \
	$(OBJDIR)\MENU_SetFont.obj \
	$(OBJDIR)\MENU_SetItem.obj \
	$(OBJDIR)\MENU_SetTextColor.obj \
	$(OBJDIR)\MESSAGEBOX.obj \
	$(OBJDIR)\MULTIEDIT.obj \
	$(OBJDIR)\MULTIEDIT_Create.obj \
	$(OBJDIR)\MULTIEDIT_CreateIndirect.obj \
	$(OBJDIR)\MULTIPAGE.obj \
	$(OBJDIR)\MULTIPAGE_Create.obj \
	$(OBJDIR)\MULTIPAGE_CreateIndirect.obj \
	$(OBJDIR)\MULTIPAGE_Default.obj \
	$(OBJDIR)\PROGBAR.obj \
	$(OBJDIR)\PROGBAR_Create.obj \
	$(OBJDIR)\PROGBAR_CreateIndirect.obj \
	$(OBJDIR)\PROGBAR_SetBarColor.obj \
	$(OBJDIR)\PROGBAR_SetFont.obj \
	$(OBJDIR)\PROGBAR_SetMinMax.obj \
	$(OBJDIR)\PROGBAR_SetText.obj \
	$(OBJDIR)\PROGBAR_SetTextAlign.obj \
	$(OBJDIR)\PROGBAR_SetTextColor.obj \
	$(OBJDIR)\PROGBAR_SetTextPos.obj \
	$(OBJDIR)\RADIO.obj \
	$(OBJDIR)\RADIO_Create.obj \
	$(OBJDIR)\RADIO_CreateIndirect.obj \
	$(OBJDIR)\RADIO_Default.obj \
	$(OBJDIR)\RADIO_GetText.obj \
	$(OBJDIR)\RADIO_Image.obj \
	$(OBJDIR)\RADIO_SetBkColor.obj \
	$(OBJDIR)\RADIO_SetDefaultImage.obj \
	$(OBJDIR)\RADIO_SetFocusColor.obj \
	$(OBJDIR)\RADIO_SetFont.obj \
	$(OBJDIR)\RADIO_SetGroupId.obj \
	$(OBJDIR)\RADIO_SetImage.obj \
	$(OBJDIR)\RADIO_SetText.obj \
	$(OBJDIR)\RADIO_SetTextColor.obj \
	$(OBJDIR)\SCROLLBAR.obj \
	$(OBJDIR)\SCROLLBAR_Create.obj \
	$(OBJDIR)\SCROLLBAR_CreateIndirect.obj \
	$(OBJDIR)\SCROLLBAR_Defaults.obj \
	$(OBJDIR)\SCROLLBAR_GetValue.obj \
	$(OBJDIR)\SCROLLBAR_SetColor.obj \
	$(OBJDIR)\SCROLLBAR_SetWidth.obj \
	$(OBJDIR)\SLIDER.obj \
	$(OBJDIR)\SLIDER_Create.obj \
	$(OBJDIR)\SLIDER_CreateIndirect.obj \
	$(OBJDIR)\SLIDER_Default.obj \
	$(OBJDIR)\SLIDER_SetFocusColor.obj \
	$(OBJDIR)\TEXT.obj \
	$(OBJDIR)\TEXT_Create.obj \
	$(OBJDIR)\TEXT_CreateIndirect.obj \
	$(OBJDIR)\TEXT_Default.obj \
	$(OBJDIR)\TEXT_SetBkColor.obj \
	$(OBJDIR)\TEXT_SetFont.obj \
	$(OBJDIR)\TEXT_SetText.obj \
	$(OBJDIR)\TEXT_SetTextAlign.obj \
	$(OBJDIR)\TEXT_SetTextColor.obj \
	$(OBJDIR)\TEXT_SetWrapMode.obj \
	$(OBJDIR)\WIDGET.obj \
	$(OBJDIR)\WIDGET_Effect_3D1L.obj \
	$(OBJDIR)\WIDGET_Effect_3D2L.obj \
	$(OBJDIR)\WIDGET_Effect_3D.obj \
	$(OBJDIR)\WIDGET_Effect_None.obj \
	$(OBJDIR)\WIDGET_Effect_Simple.obj \
	$(OBJDIR)\WIDGET_FillStringInRect.obj \
	$(OBJDIR)\WIDGET_SetEffect.obj \
	$(OBJDIR)\WIDGET_SetWidth.obj \
	$(OBJDIR)\WINDOW.obj \
	$(OBJDIR)\WINDOW_Default.obj

WMOBJS = $(OBJDIR)\WM.obj \
	$(OBJDIR)\WM__ForEachDesc.obj \
	$(OBJDIR)\WM__GetFirstSibling.obj \
	$(OBJDIR)\WM__GetFocussedChild.obj \
	$(OBJDIR)\WM__GetLastSibling.obj \
	$(OBJDIR)\WM__GetOrg_AA.obj \
	$(OBJDIR)\WM__GetPrevSibling.obj \
	$(OBJDIR)\WM__IsAncestor.obj \
	$(OBJDIR)\WM__IsChild.obj \
	$(OBJDIR)\WM__IsEnabled.obj \
	$(OBJDIR)\WM__NotifyVisChanged.obj \
	$(OBJDIR)\WM__Screen2Client.obj \
	$(OBJDIR)\WM__SendMessage.obj \
	$(OBJDIR)\WM__SendMessageIfEnabled.obj \
	$(OBJDIR)\WM__SendMessageNoPara.obj \
	$(OBJDIR)\WM__UpdateChildPositions.obj \
	$(OBJDIR)\WM_AttachWindow.obj \
	$(OBJDIR)\WM_BringToBottom.obj \
	$(OBJDIR)\WM_BringToTop.obj \
	$(OBJDIR)\WM_Broadcast.obj \
	$(OBJDIR)\WM_CheckScrollPos.obj \
	$(OBJDIR)\WM_CriticalHandle.obj \
	$(OBJDIR)\WM_DIAG.obj \
	$(OBJDIR)\WM_EnableWindow.obj \
	$(OBJDIR)\WM_ForEachDesc.obj \
	$(OBJDIR)\WM_GetBkColor.obj \
	$(OBJDIR)\WM_GetCallback.obj \
	$(OBJDIR)\WM_GetClientRect.obj \
	$(OBJDIR)\WM_GetClientWindow.obj \
	$(OBJDIR)\WM_GetDesktopWindow.obj \
	$(OBJDIR)\WM_GetDesktopWindowEx.obj \
	$(OBJDIR)\WM_GetDiagInfo.obj \
	$(OBJDIR)\WM_GetDialogItem.obj \
	$(OBJDIR)\WM_GetFirstChild.obj \
	$(OBJDIR)\WM_GetFlags.obj \
	$(OBJDIR)\WM_GetFocussedWindow.obj \
	$(OBJDIR)\WM_GetId.obj \
	$(OBJDIR)\WM_GetInsideRect.obj \
	$(OBJDIR)\WM_GetInsideRectExScrollbar.obj \
	$(OBJDIR)\WM_GetInvalidRect.obj \
	$(OBJDIR)\WM_GetNextSibling.obj \
	$(OBJDIR)\WM_GetOrg.obj \
	$(OBJDIR)\WM_GetParent.obj \
	$(OBJDIR)\WM_GetPrevSibling.obj \
	$(OBJDIR)\WM_GetScrollbar.obj \
	$(OBJDIR)\WM_GetScrollPartner.obj \
	$(OBJDIR)\WM_GetScrollPos.obj \
	$(OBJDIR)\WM_GetScrollState.obj \
	$(OBJDIR)\WM_GetWindowRect.obj \
	$(OBJDIR)\WM_GetWindowSize.obj \
	$(OBJDIR)\WM_HasCaptured.obj \
	$(OBJDIR)\WM_HasFocus.obj \
	$(OBJDIR)\WM_Hide.obj \
	$(OBJDIR)\WM_InvalidateArea.obj \
	$(OBJDIR)\WM_IsCompletelyVisible.obj \
	$(OBJDIR)\WM_IsEnabled.obj \
	$(OBJDIR)\WM_IsFocussable.obj \
	$(OBJDIR)\WM_IsVisible.obj \
	$(OBJDIR)\WM_IsWindow.obj \
	$(OBJDIR)\WM_MakeModal.obj \
	$(OBJDIR)\WM_Move.obj \
	$(OBJDIR)\WM_MoveChildTo.obj \
	$(OBJDIR)\WM_NotifyParent.obj \
	$(OBJDIR)\WM_OnKey.obj \
	$(OBJDIR)\WM_Paint.obj \
	$(OBJDIR)\WM_PaintWindowAndDescs.obj \
	$(OBJDIR)\WM_PID__GetPrevState.obj \
	$(OBJDIR)\WM_ResizeWindow.obj \
	$(OBJDIR)\WM_Screen2Win.obj \
	$(OBJDIR)\WM_SendMessageNoPara.obj \
	$(OBJDIR)\WM_SendToParent.obj \
	$(OBJDIR)\WM_SetAnchor.obj \
	$(OBJDIR)\WM_SetCallback.obj \
	$(OBJDIR)\WM_SetCapture.obj \
	$(OBJDIR)\WM_SetCaptureMove.obj \
	$(OBJDIR)\WM_SetCreateFlags.obj \
	$(OBJDIR)\WM_SetDesktopColor.obj \
	$(OBJDIR)\WM_SetFocus.obj \
	$(OBJDIR)\WM_SetFocusOnNextChild.obj \
	$(OBJDIR)\WM_SetFocusOnPrevChild.obj \
	$(OBJDIR)\WM_SetId.obj \
	$(OBJDIR)\WM_SetpfPollPID.obj \
	$(OBJDIR)\WM_SetScrollbar.obj \
	$(OBJDIR)\WM_SetScrollPos.obj \
	$(OBJDIR)\WM_SetScrollState.obj \
	$(OBJDIR)\WM_SetSize.obj \
	$(OBJDIR)\WM_SetTrans.obj \
	$(OBJDIR)\WM_SetTransState.obj \
	$(OBJDIR)\WM_SetUserClipRect.obj \
	$(OBJDIR)\WM_SetWindowPos.obj \
	$(OBJDIR)\WM_SetXSize.obj \
	$(OBJDIR)\WM_SetYSize.obj \
	$(OBJDIR)\WM_Show.obj \
	$(OBJDIR)\WM_SIM.obj \
	$(OBJDIR)\WM_StayOnTop.obj \
	$(OBJDIR)\WM_Timer.obj \
	$(OBJDIR)\WM_TimerExternal.obj \
	$(OBJDIR)\WM_UpdateWindowAndDescs.obj \
	$(OBJDIR)\WM_UserData.obj \
	$(OBJDIR)\WM_Validate.obj \
	$(OBJDIR)\WM_ValidateWindow.obj \
	$(OBJDIR)\WMMemDev.obj \
	$(OBJDIR)\WMTouch.obj

LCDOBJS = $(OBJDIR)\LCDWinFrameBuffer.obj
GUIXOBJS = $(OBJDIR)\GUI_X.obj


all: $(UCGUI)

$(UCGUI): $(ucGui1) $(ucGui2) $(ucGui3) $(ucGui4)
	$(MKLIB)"OUTPUT\LIB\ucGUILIB.lib" $(ucGui1) $(ucGui2) $(ucGui3) $(ucGui4)

$(ucGui1): $(AAOBJS) $(CCOBJS) $(CMOBJS) $(FONTOBJS) $(MDOBJS) $(LCDOBJS) $(GUIXOBJS)
	$(MKLIB)"OUTPUT\LIB\temp1.lib" $(AAOBJS) $(CCOBJS) $(CMOBJS) $(FONTOBJS) $(MDOBJS) $(LCDOBJS) $(GUIXOBJS)

$(ucGui2): $(COREOBJS)
	$(MKLIB)"OUTPUT\LIB\temp2.lib" $(COREOBJS)

$(ucGui3): $(WGOBJS)
	$(MKLIB)"OUTPUT\LIB\temp3.lib" $(WGOBJS)

$(ucGui4): $(WMOBJS) $(WGOBJS2)
	$(MKLIB)"OUTPUT\LIB\temp4.lib" $(WMOBJS) $(WGOBJS2)

#$(AAOBJS) $(CCOBJS) $(CMOBJS) $(COREOBJS) $(FONTOBJS) $(MDOBJS) $(WGOBJS) $(WMOBJS) $(LCDOBJS) $(GUIXOBJS)
#	$(MKLIB) $(AAOBJS) $(CCOBJS) $(CMOBJS) $(COREOBJS) $(FONTOBJS) $(MDOBJS) $(WGOBJS) $(WMOBJS) $(LCDOBJS) $(GUIXOBJS)

AADIR = GUI\AntiAlias
CCDIR = GUI\ConvertColor
CMDIR = GUI\ConvertMono
COREDIR = GUI\Core
FONTDIR = GUI\Font
MDDIR = GUI\MemDev
WGDIR = GUI\Widget
WMDIR = GUI\WM
LCDDIR = GUI\LCDDriver
GUIXDIR = GUI\GUI_X

$(AAOBJS): $(OBJDIR)\\%.obj: $(AADIR)\\%.c
	$(CPP) $(CPP_PROJ) $<

$(CCOBJS): $(OBJDIR)\\%.obj: $(CCDIR)\\%.c
	$(CPP) $(CPP_PROJ) $<

$(CMOBJS): $(OBJDIR)\\%.obj: $(CMDIR)\\%.c
	$(CPP) $(CPP_PROJ) $<

$(COREOBJS): $(OBJDIR)\\%.obj: $(COREDIR)\\%.c
	$(CPP) $(CPP_PROJ) $<

$(FONTOBJS): $(OBJDIR)\\%.obj: $(FONTDIR)\\%.c
	$(CPP) $(CPP_PROJ) $<

$(MDOBJS): $(OBJDIR)\\%.obj: $(MDDIR)\\%.c
	$(CPP) $(CPP_PROJ) $<

$(WGOBJS): $(OBJDIR)\\%.obj: $(WGDIR)\\%.c
	$(CPP) $(CPP_PROJ) $<

$(WGOBJS2): $(OBJDIR)\\%.obj: $(WGDIR)\\%.c
	$(CPP) $(CPP_PROJ) $<

$(WMOBJS): $(OBJDIR)\\%.obj: $(WMDIR)\\%.c
	$(CPP) $(CPP_PROJ) $<

$(LCDOBJS): $(OBJDIR)\\%.obj: $(LCDDIR)\\%.c
	$(CPP) $(CPP_PROJ) $<

$(GUIXOBJS): $(OBJDIR)\\%.obj: $(GUIXDIR)\\%.c
	$(CPP) $(CPP_PROJ) $<

clean:

endif

