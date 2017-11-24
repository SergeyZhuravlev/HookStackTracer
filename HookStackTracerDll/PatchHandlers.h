#pragma once

extern void** CreateEventExAOldHandlerPtr;
extern void* CreateEventExAOldHandler;
void CreateEventExAHandler();
extern void** CreateEventExAHandlerPtr;

extern void** CreateEventExWOldHandlerPtr;
extern void* CreateEventExWOldHandler;
void CreateEventExWHandler();
extern void** CreateEventExWHandlerPtr;

//=======================================

extern void** CreateEventAOldHandlerPtr;
extern void* CreateEventAOldHandler;
void CreateEventAHandler();
extern void** CreateEventAHandlerPtr;

extern void** CreateEventWOldHandlerPtr;
extern void* CreateEventWOldHandler;
void CreateEventWHandler();
extern void** CreateEventWHandlerPtr;

//=======================================

extern void** CloseHandleOldHandlerPtr;
extern void* CloseHandleOldHandler;
void CloseHandleHandler();
extern void** CloseHandleHandlerPtr;
