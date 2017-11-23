#pragma once

extern void** CreateEventExAOldHandlerPtr;
extern void* CreateEventExAOldHandler;
void CreateEventExAHandler();
extern void** CreateEventExAHandlerPtr;

extern void** CreateEventExWOldHandlerPtr;
extern void* CreateEventExWOldHandler;
void CreateEventExWHandler();
extern void** CreateEventExWHandlerPtr;

extern void** CloseHandleOldHandlerPtr;
extern void* CloseHandleOldHandler;
void CloseHandleHandler();
extern void** CloseHandleHandlerPtr;
