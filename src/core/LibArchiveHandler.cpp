#include "playarchive/LibArchiveHandler.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <io.h>
#define open _open
#define read _read
#define close _close
#define stat _stat
#else
#include <unistd.h>
#endif

namespace fs = std::filesystem;

namespace PlayArchive {

LibArchiveHandler::LibArchiveHandler(const std::string& format) : m_format(format) {}

bool LibArchiveHandler::create(const std::string& archivePath, 
                               const std::vector<std::string>& sourcePaths,
                               ProgressCallback progress) {
    struct archive *a;
    struct archive_entry *entry;
    struct stat st;
    char buff[16384];
    int len;
    int fd;

    a = archive_write_new();
    if (m_format == "zip") archive_write_set_format_zip(a);
    else if (m_format == "7z") archive_write_set_format_7zip(a);
    else archive_write_set_format_pax_restricted(a); // Default to tar

    archive_write_open_filename(a, archivePath.c_str());

    for (const auto& source : sourcePaths) {
        for (const auto& p : fs::recursive_directory_iterator(source)) {
            std::string relPath = fs::relative(p.path(), source).string();
            
            stat(p.path().string().c_str(), &st);
            entry = archive_entry_new();
            archive_entry_set_pathname(entry, relPath.c_str());
            archive_entry_set_size(entry, st.st_size);
            archive_entry_set_filetype(entry, AE_IFREG);
            archive_entry_set_perm(entry, 0644);
            archive_entry_copy_stat(entry, &st);
            archive_write_header(a, entry);

            fd = open(p.path().string().c_str(), O_RDONLY | O_BINARY);
            len = read(fd, buff, sizeof(buff));
            while (len > 0) {
                archive_write_data(a, buff, len);
                len = read(fd, buff, sizeof(buff));
            }
            close(fd);
            archive_entry_free(entry);

            if (progress) progress(0, 0, relPath);
        }
    }

    archive_write_close(a);
    archive_write_free(a);
    return true;
}

bool LibArchiveHandler::extract(const std::string& archivePath, 
                                const std::string& destinationPath,
                                ProgressCallback progress) {
    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int flags;
    int r;

    flags = ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS;

    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);

    if ((r = archive_read_open_filename(a, archivePath.c_str(), 10240))) return false;

    for (;;) {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF) break;
        if (r < ARCHIVE_OK) return false;

        const char* currentFile = archive_entry_pathname(entry);
        fs::path fullOutputPath = fs::path(destinationPath) / currentFile;
        archive_entry_set_pathname(entry, fullOutputPath.string().c_str());

        r = archive_write_header(ext, entry);
        if (r < ARCHIVE_OK) return false;
        else if (archive_entry_size(entry) > 0) {
            const void *buff;
            size_t size;
            la_int64_t offset;
            for (;;) {
                r = archive_read_data_block(a, &buff, &size, &offset);
                if (r == ARCHIVE_EOF) break;
                if (r < ARCHIVE_OK) return false;
                archive_write_data_block(ext, buff, size, offset);
            }
        }
        archive_write_finish_entry(ext);
        
        if (progress) progress(0, 0, currentFile);
    }

    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);
    return true;
}

std::vector<ArchiveEntry> LibArchiveHandler::list(const std::string& archivePath) {
    std::vector<ArchiveEntry> entries;
    struct archive *a;
    struct archive_entry *entry;
    int r;

    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    if (archive_read_open_filename(a, archivePath.c_str(), 10240) != ARCHIVE_OK) return entries;

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        ArchiveEntry ae;
        ae.path = archive_entry_pathname(entry);
        ae.size = archive_entry_size(entry);
        ae.isDirectory = (archive_entry_filetype(entry) == AE_IFDIR);
        entries.push_back(ae);
        archive_read_data_skip(a);
    }

    archive_read_free(a);
    return entries;
}

} // namespace PlayArchive
