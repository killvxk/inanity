exports.configureCompiler = function(objectFile, compiler) {
	// объектные файлы: <conf>/object
	var a = /^[^\/]+\/([^\/]+)$/.exec(objectFile);
	compiler.setSourceFile(a[1] + '.cpp');
	compiler.addMacro('INANITY_LIB');
};

/**
 * структура проекта
 */
var libraries = {
	// ****** базовая библиотека
	'libinanity2-base': {
		objects: [
		// совсем общее
		'Object', 'ManagedHeap', 'Strings', 'Exception',
		// общее: файлы и потоки
		'File', 'EmptyFile', 'PartFile', 'MemoryFile', 'OutputStream', 'FileInputStream',
		// файловые системы
		'FileSystem', 'FolderFileSystem', 'Handle', 'DiskInputStream', 'DiskOutputStream']
	},
	// ******* сетевая библиотека
	'libinanity2-net': {
		objects: [
		// базовая часть
		'EventLoop',
		// сокеты
		'Socket', 'ClientSocket', 'ServerSocket',
		// HTTP
		'HttpClient', 'HttpResponseStream', 'http_parser']
	},
	// ******* скрипты на lua
	'libinanity2-lua': {
		objects: ['LuaState']
	}
};

exports.configureComposer = function(libraryFile, composer) {
	// файлы библиотек: <conf>/library
	var a = /^([^\/]+\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	var library = libraries[a[2]];
	for ( var i = 0; i < library.objects.length; ++i)
		composer.addObjectFile(confDir + library.objects[i]);
};
