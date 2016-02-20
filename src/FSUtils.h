#ifndef FSUTILS_H
#define FSUTILS_H
#include <QFileInfo>
#include <QDir>

class FSUtils
{
public:
	static const quint64 KB = 1024;
	static const quint64 MB = 1024 * KB;
	static const quint64 GB = 1024 * MB;

private:
	// Deny instancing
	FSUtils()
	{	
	}

public:
	static quint64 computeSize(const QString & str)
	{	    
	    QFileInfo str_info(str);

	    if (str_info.isDir())
	    {
	        QDir dir(str);
	        QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);

	        quint64 sizex = 0;
	        foreach (QFileInfo fileInfo, list)
                sizex += computeSize(fileInfo.absoluteFilePath());

            return sizex;
	    }

	    return str_info.size();
	} 

	static QString formatSize(const quint64 sizeInBytes)
	{
		if (sizeInBytes < KB)
			return QString("%1 bytes").arg(sizeInBytes);

		if (sizeInBytes < MB)
			return QString("%1 KB").arg(QString::number(sizeInBytes / (double)KB, 'f', 2));;

		if (sizeInBytes < GB)
			return QString("%1 MB").arg(QString::number(sizeInBytes / (double)MB, 'f', 2));;
	
		return QString("%1 GB").arg(QString::number(sizeInBytes / (double)GB, 'f', 2));;
	}
};

#endif