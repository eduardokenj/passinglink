#pragma once

#ifndef LOG_LEVEL
#error LOG_LEVEL must be defined before including profiling.h.
#endif

#if defined(CONFIG_PASSINGLINK_PROFILING)
template<int Frequency = 1>
struct Profiler {
  void begin() {
    begin_cycle_ = k_cycle_get_32();
  }

  void end(const char* name) {
    u32_t end_cycle = k_cycle_get_32();
    u32_t diff = end_cycle - begin_cycle_;
    times_[times_count_++] = diff;
    if (times_count_ == Frequency) {
      times_count_ = 0;
      u64_t total = 0;
      u32_t min = UINT32_MAX;
      u32_t max = 0;
      for (auto time : times_) {
        total += time;
        if (time < min) {
          min = time;
        }
        if (time > max) {
          max = time;
        }
      }

      u32_t average = total / Frequency;

      LOG_INF("%s: avg = %u, min = %u, max = %u", name, average, min, max);
    }
  }

  u32_t times_[Frequency];
  size_t times_count_;

  u32_t begin_cycle_;
};

template<int Frequency>
struct ScopedProfile {
  explicit ScopedProfile(Profiler<Frequency>& profiler, const char* name)
      : profiler_(profiler), name_(name) {
    profiler_.begin();
  }

  ~ScopedProfile() {
    profiler_.end(name_);
  }

  Profiler<Frequency>& profiler_;
  const char* name_;
};
#else
template<int Frequency>
struct Profiler {};

template<int Frequency>
struct ScopedProfile {
  explicit ScopedProfile(Profiler<Frequency>& profiler, const char* name) {}
};
#endif

#define PROFILE(name, frequency)         \
  static Profiler<frequency> __profiler; \
  ScopedProfile<frequency> __scoped_profiler(__profiler, (name))