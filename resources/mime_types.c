#include "mime_types.h"

int mime_add(char *extension , char *value){
    char * val = malloc(strlen(value)+1);
    strcpy(val , value);
    map_add(&mime_map , extension , val , STR_TYPE);
    return 0;
}

char* mime_get(char * extension){
    char * res = (char *)map_get(&mime_map , extension , STR_TYPE);
    if(!res)
        return NULL;
    return res;
}

void mimes(){
    mime_add("aac", "audio/aac");
    mime_add("abw", "application/x-abiword");
    mime_add("arc", "application/x-freearc");
    mime_add("avi", "video/x-msvideo");
    mime_add("azw", "application/vnd.amazon.ebook");
    mime_add("bin", "application/octet-stream");
    mime_add("bmp", "image/bmp");
    mime_add("bz", "application/x-bzip");
    mime_add("bz2", "application/x-bzip2");
    mime_add("cda", "application/x-cdf");
    mime_add("csh", "application/x-csh");
    mime_add("css", "text/css");
    mime_add("csv", "text/csv");
    mime_add("doc", "application/msword");
    mime_add("docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document");
    mime_add("eot", "application/vnd.ms-fontobject");
    mime_add("epub", "application/epub+zip");
    mime_add("gz", "application/gzip");
    mime_add("gif", "image/gif");
    mime_add("htm", "text/html");
    mime_add("html", "text/html");
    mime_add("ico", "image/vnd.microsoft.icon");
    mime_add("ics", "text/calendar");
    mime_add("jar", "application/java-archive");
    mime_add("jpeg", "image/jpeg");
    mime_add("jpg", "image/jpeg");
    mime_add("js", "text/javascript");
    mime_add("json", "application/json");
    mime_add("jsonld", "application/ld+json");
    mime_add("mid", "audio/midi");
    mime_add("midi", "audio/midi");
    mime_add("mjs", "text/javascript");
    mime_add("mp3", "audio/mpeg");
    mime_add("mp4", "video/mp4");
    mime_add("mpeg", "video/mpeg");
    mime_add("mpkg", "application/vnd.apple.installer+xml");
    mime_add("odp", "application/vnd.oasis.opendocument.presentation");
    mime_add("ods", "application/vnd.oasis.opendocument.spreadsheet");
    mime_add("odt", "application/vnd.oasis.opendocument.text");
    mime_add("oga", "audio/ogg");
    mime_add("ogv", "video/ogg");
    mime_add("ogx", "application/ogg");
    mime_add("opus", "audio/opus");
    mime_add("otf", "font/otf");
    mime_add("png", "image/png");
    mime_add("pdf", "application/pdf");
    mime_add("php", "application/x-httpd-php");
    mime_add("ppt", "application/vnd.ms-powerpoint");
    mime_add("pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation");
    mime_add("rar", "application/vnd.rar");
    mime_add("rtf", "application/rtf");
    mime_add("sh", "application/x-sh");
    mime_add("svg", "image/svg+xml");
    mime_add("tar", "application/x-tar");
    mime_add("tif", "image/tiff");
    mime_add("tiff", "image/tiff");
    mime_add("ts", "video/mp2t");
    mime_add("ttf", "font/ttf");
    mime_add("txt", "text/plain");
    mime_add("vsd", "application/vnd.visio");
    mime_add("wav", "audio/wav");
    mime_add("weba", "audio/webm");
    mime_add("webm", "video/webm");
    mime_add("webp", "image/webp");
    mime_add("woff", "font/woff");
    mime_add("woff2", "font/woff2");
    mime_add("xhtml", "application/xhtml+xml");
    mime_add("xls", "application/vnd.ms-excel");
    mime_add("xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
    mime_add("xml", "application/xml");
    mime_add("xul", "application/vnd.mozilla.xul+xml");
    mime_add("zip", "application/zip");
    mime_add("3gp", "video/3gpp");
    mime_add("3g2", "video/3gpp2");
    mime_add("7z", "application/x-7z-compressed");
}

