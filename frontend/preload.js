const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('api', {
    cppAction: (cmd) => ipcRenderer.invoke('cpp-action', cmd),
    getAllTables: () => ipcRenderer.invoke('get-all-tables'),
    showDatabase: () => ipcRenderer.invoke('show-database'),
    goBack: () => ipcRenderer.invoke('go-back'),
    logAction: (msg) => ipcRenderer.invoke('log-action', msg),
    getCustomerId: () => ipcRenderer.invoke('get-customer-id')
});