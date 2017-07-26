#include <cstdio>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <malloc.h>
#include <map>
#include <memory>
#include <unistd.h>
#include <vector>
#include <iomanip>
#include <chrono>

#define SEC_TO_USEC 1000000.0

using long_size_t = unsigned long long;
using timepoint_t = std::chrono::time_point<std::chrono::system_clock>;
using duration_t = std::chrono::duration<long double>;
using test_func_t = std::function<const long double(const int&,
                                                    char*,
                                                    const long_size_t&,
                                                    const long_size_t&)>;

void generate_buffer(char *buf, const unsigned long long& buffer_size) {
    for(int j = 0; j < buffer_size; ++j) {
        buf[j] = 'a' + (j % 26);
    }
}

int initialize_file(const std::string& io_method,
                    const char* buf,
                    const long_size_t& buffer_size,
                    const long_size_t& num_itrs) {
  auto file_flags = O_RDWR | O_CREAT | O_SYNC;
  if (io_method == "direct")
    file_flags |= O_DIRECT;
  int fd = open("test_file.tmp", file_flags, 0664);
  for (long_size_t i = 0; i < num_itrs; i++) {
    if (write(fd, buf, buffer_size) < 0) {
      std::cerr << "Write failed." << std::endl;
      perror("Write");
      exit(-1);
    }
  }
  lseek(fd, 0, SEEK_SET);
  return fd;
}

const long double test_write_simple(const int& fd,
                                    char* buf,
                                    const long_size_t& buffer_size,
                                    const long_size_t& num_itrs) {
  timepoint_t start, end;
  start = std::chrono::system_clock::now();
  for (long_size_t i = 0; i < num_itrs; i++) {
    if (write(fd, buf, buffer_size) < 0) {
      std::cerr << "Write failed." << std::endl;
      perror("test_write_simple");
      exit(-1);
    }
  }
  end = std::chrono::system_clock::now();
  duration_t duration = end - start;
  return duration.count() * SEC_TO_USEC / static_cast<long double>(num_itrs);
}

const long double test_read_simple(const int& fd,
                                   char* buf,
                                   const long_size_t& buffer_size,
                                   const long_size_t& num_itrs) {
  lseek(fd, 0, SEEK_SET);
  timepoint_t start, end;
  start = std::chrono::system_clock::now();
  for (long_size_t i = 0; i < num_itrs; i++) {
    if (read(fd, buf, buffer_size) < 0) {
      std::cerr << "Read failed." << std::endl;
      perror("test_read_simple");
      exit(-1);
    }
  }
  end = std::chrono::system_clock::now();
  duration_t duration = end - start;
  return duration.count() * SEC_TO_USEC / static_cast<long double>(num_itrs);
}

const long double test_write_direct(const int& fd,
                                    char* buf,
                                    const long_size_t& buffer_size,
                                    const long_size_t& num_itrs) {
  timepoint_t start, end;
  start = std::chrono::system_clock::now();
  for (long_size_t i = 0; i < num_itrs; i++) {
    if (write(fd, buf, buffer_size) < 0) {
      std::cerr << "Write failed." << std::endl;
      perror("test_write_direct");
      exit(-1);
    }
  }
  end = std::chrono::system_clock::now();
  duration_t duration = end - start;
  return duration.count() * SEC_TO_USEC / static_cast<long double>(num_itrs);
}

const long double test_read_direct(const int& fd,
                                   char* buf,
                                   const long_size_t& buffer_size,
                                   const long_size_t& num_itrs) {
  lseek(fd, 0, SEEK_SET);
  timepoint_t start, end;
  start = std::chrono::system_clock::now();
  for (long_size_t i = 0; i < num_itrs; i++) {
    if (read(fd, buf, buffer_size) < 0) {
      std::cerr << "Read failed." << std::endl;
      perror("test_read_direct");
      exit(-1);
    }
  }
  end = std::chrono::system_clock::now();
  duration_t duration = end - start;
  return duration.count() * SEC_TO_USEC / static_cast<long double>(num_itrs);
}

const long double test_write_p(const int& fd,
                               char* buf,
                               const long_size_t& buffer_size,
                               const long_size_t& num_itrs) {
  timepoint_t start, end;
  start = std::chrono::system_clock::now();
  for (long_size_t i = 0; i < num_itrs; i++) {
    if (pwrite(fd, buf, buffer_size, i * buffer_size) < 0) {
      std::cerr << "Write failed." << std::endl;
      perror("test_write_p");
      exit(-1);
    }
  }
  end = std::chrono::system_clock::now();
  duration_t duration = end - start;
  return duration.count() * SEC_TO_USEC / static_cast<long double>(num_itrs);
}

const long double test_read_p(const int& fd,
                              char* buf,
                              const long_size_t& buffer_size,
                              const long_size_t& num_itrs) {
  timepoint_t start, end;
  start = std::chrono::system_clock::now();
  for (long_size_t i = 0; i < num_itrs; i++) {
    if (pread(fd, buf, buffer_size, i * buffer_size) < 0) {
      std::cerr << "Read failed." << std::endl;
      perror("test_read_p");
      exit(-1);
    }
  }
  end = std::chrono::system_clock::now();
  duration_t duration = end - start;
  return duration.count() * SEC_TO_USEC / static_cast<long double>(num_itrs);
}

const long double test_write_mmap(const int& fd,
                                  char* buf,
                                  const long_size_t& buffer_size,
                                  const long_size_t& num_itrs) {
  //
}

const long double test_read_mmap(const int& fd,
                                 char* buf,
                                 const long_size_t& buffer_size,
                                 const long_size_t& num_itrs) {
  //
}

std::map<std::string, test_func_t> test_func_map {
  {"simple_write", test_func_t(test_write_simple)},
  {"simple_read", test_func_t(test_read_simple)},
  {"direct_write", test_func_t(test_write_direct)},
  {"direct_read", test_func_t(test_read_direct)},
  {"p_write", test_func_t(test_write_p)},
  {"p_read", test_func_t(test_read_p)},
  {"mmap_write", test_func_t(test_write_mmap)},
  {"mmap_read", test_func_t(test_read_mmap)}
};

test_func_t test_write(const std::string& io_method) {
  return test_func_map[io_method + "_write"];
}

test_func_t test_read(const std::string& io_method) {
  return test_func_map[io_method + "_read"];
}

int main(int argc, char const *argv[]) {
  const auto io_method = std::string(argv[1]);
  const auto buffer_size = std::stoull(argv[2], nullptr, 10);
  const auto num_itrs = std::stoull(argv[3], nullptr, 10);
  std::cout << "\n============================================="
            << "\nTesting I/O latency with " << io_method
            << "\nBuffer size: " << buffer_size
            << "\nNumber of repeats: " << num_itrs
            << "\n=============================================" << std::endl;
  auto buf = (char *)memalign(buffer_size, buffer_size);
  generate_buffer(buf, buffer_size);
  auto fd = initialize_file(io_method, buf, buffer_size, num_itrs);

  std::cout << "Write: " << std::fixed
            << test_write(io_method)(fd, buf, buffer_size, num_itrs) << "us\n"
            << "Read: " << std::fixed
            << test_read(io_method)(fd, buf, buffer_size, num_itrs) << "us"
            << "\n=============================================\n" << std::endl;
  return 0;
}
