#pragma once

#include <sys/types.h>

#include <kernel.h>

#include "output/usb/hid.h"
#include "types.h"

namespace ps4 {

class Hid : public ::Hid {
 public:
  virtual const char* Name() const override final { return "PS4"; }

  virtual span<const u8_t> ReportDescriptor() const override final;
  ssize_t GetFeatureReport(u8_t report_id, span<u8_t> buf);
  ssize_t GetInputReport(u8_t report_id, span<u8_t> buf);
  virtual ssize_t GetReport(optional<HidReportType> report_type, u8_t report_id,
                            span<u8_t> buf) override final;
  virtual bool SetReport(optional<HidReportType> report_type, u8_t report_id,
                         span<u8_t> data) override final;

  // PS4 Hid is the last, and will always fail, but give it a delay to make the LED visible.
  virtual u32_t ProbeDelay() { return k_ms_to_ticks_ceil32(1000); }

 private:
  uint8_t last_report_counter_ = 0;
};

}  // namespace ps4
