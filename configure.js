exports.configureCompiler = function(objectFile, compiler) {
	// объектные файлы: <conf>/object
	var a = /^([^\/]+)\/([^\/]+)$/.exec(objectFile);
	compiler.configuration = a[1];
	compiler.setSourceFile(a[2] + '.cpp');
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
		'File', 'EmptyFile', 'PartFile', 'MemoryFile', 'OutputStream', 'FileInputStream', 'StreamReader', 'StreamWriter', 'BufferedInputStream', 'BufferedOutputStream',
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
	var a = /^(([^\/]+)\/)([^\/]+)$/.exec(libraryFile);
	var confDir = a[1];
	composer.configuration = a[2];
	var library = libraries[a[3]];
	for ( var i = 0; i < library.objects.length; ++i)
		composer.addObjectFile(confDir + library.objects[i]);
};
