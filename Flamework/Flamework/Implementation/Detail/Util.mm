//
//  Util.m
//  Framework
//
//  Created by David Steiner on 3/4/13.
//
//

#include "Util.h"

namespace util
{
    void log(const std::string &arg, LogMode mode)
    {
        std::string tag;
        switch (mode)
        {
            case LM_INFO:
                tag = "INFO";
                break;
            case LM_WARNING:
                tag = "WARNING";
                break;
            case LM_ERROR:
                tag = "ERROR";
                break;
            default:
                break;
        }
        std::string taggedMsg = tag + ": " + arg;
        
        NSString *msg = [NSString stringWithCString:taggedMsg.c_str()
                                           encoding:[NSString defaultCStringEncoding]];
        NSLog(@"%@", msg);
    }
    
    std::string getFilePath(const std::string &fileName)
    {
        NSString *fullFile = [NSString stringWithCString:fileName.c_str()
                                                   encoding:[NSString defaultCStringEncoding]];
        
        NSString* path = [[fullFile lastPathComponent] stringByDeletingPathExtension];
        NSString* extension = [fullFile pathExtension];
        
        NSURL *url = [NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:path ofType:extension]];
        
        NSString *urlString = [url path];
        
        return std::string([urlString UTF8String]);
    }
    
    bool fileExists(const std::string &fileName)
    {
        NSString *fullFile = [NSString stringWithCString:fileName.c_str()
                                                encoding:[NSString defaultCStringEncoding]];
        
        NSFileManager *fileManager = [NSFileManager defaultManager];
//        NSString *documentsDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
        NSString *path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent: fullFile];
        
        return [fileManager fileExistsAtPath:path];
    }
}