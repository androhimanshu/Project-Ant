#include "content/browser/ant/ant_service_impl.h"

#include "base/ant_values.h"
#include "base/logging.h"
#include "base/no_destructor.h"
#include "base/version.h"

namespace content {

AntServiceImpl::AntServiceImpl() = default;
AntServiceImpl::~AntServiceImpl() = default;

// static
AntServiceImpl* AntServiceImpl::GetInstance() {
  static base::NoDestructor<AntServiceImpl> instance;
  return instance.get();
}

void AntServiceImpl::BindReceiver(
    mojo::PendingReceiver<blink::mojom::AntService> receiver) {
  receivers_.Add(this, std::move(receiver));
}

void AntServiceImpl::SetValue(int32_t v, SetValueCallback cb) {
  stored_value_ = v;
  base::AntValues::GetInstance()->value.store(v);
  for (auto& o : observers_) o->OnValueChanged(v);
  std::move(cb).Run("ok");
}
void AntServiceImpl::GetValue(GetValueCallback cb) {
  std::move(cb).Run(stored_value_);
}

void AntServiceImpl::AddObserver(
    mojo::PendingRemote<blink::mojom::AntValueObserver> observer) {
  observers_.Add(std::move(observer));
}

void AntServiceImpl::SetString(const std::string& t, SetStringCallback cb) {
  stored_string_ = t;
  base::AntValues::GetInstance()->string_val = t;
  for (auto& o : observers_) o->OnStringChanged(t);
  std::move(cb).Run("ok");
}
void AntServiceImpl::GetString(GetStringCallback cb) {
  std::move(cb).Run(stored_string_);
}

void AntServiceImpl::SetGpu(const std::string& v, SetGpuCallback cb) {
  stored_gpu_ = v;
  base::AntValues::GetInstance()->gpu = v;
  std::move(cb).Run("ok");
}
void AntServiceImpl::GetGpu(GetGpuCallback cb) {
  std::move(cb).Run(stored_gpu_);
}

void AntServiceImpl::SetModel(const std::string& v, SetModelCallback cb) {
  stored_model_ = v;
  base::AntValues::GetInstance()->model = v;
  std::move(cb).Run("ok");
}
void AntServiceImpl::GetModel(GetModelCallback cb) {
  std::move(cb).Run(stored_model_);
}

void AntServiceImpl::SetAversion(const std::string& v, SetAversionCallback cb) {
  stored_aversion_ = v;
  base::AntValues::GetInstance()->aversion = v;
  std::move(cb).Run("ok");
}
void AntServiceImpl::GetAversion(GetAversionCallback cb) {
  std::move(cb).Run(stored_aversion_);
}

void AntServiceImpl::SetBversion(const std::string& v, SetBversionCallback cb) {
  stored_bversion_ = v;
  base::AntValues::GetInstance()->bversion = v;
  // Parse and store Version object so version_info.cc can return a reference.
  // Must be dotted numbers e.g. "120.0.6099.71"
  base::AntValues::GetInstance()->bversion_parsed = base::Version(v);
  std::move(cb).Run("ok");
}
void AntServiceImpl::GetBversion(GetBversionCallback cb) {
  std::move(cb).Run(stored_bversion_);
}

void AntServiceImpl::SetVendor(const std::string& v, SetVendorCallback cb) {
  stored_vendor_ = v;
  base::AntValues::GetInstance()->vendor = v;
  std::move(cb).Run("ok");
}
void AntServiceImpl::GetVendor(GetVendorCallback cb) {
  std::move(cb).Run(stored_vendor_);
}

void AntServiceImpl::SetCpu(int32_t v, SetCpuCallback cb) {
  stored_cpu_ = v;
  base::AntValues::GetInstance()->cpu.store(v);
  std::move(cb).Run("ok");
}
void AntServiceImpl::GetCpu(GetCpuCallback cb) {
  std::move(cb).Run(stored_cpu_);
}

void AntServiceImpl::SetBrand(const std::string& v, SetBrandCallback cb) {
  stored_brand_ = v;
  base::AntValues::GetInstance()->brand = v;
  std::move(cb).Run("ok");
}
void AntServiceImpl::GetBrand(GetBrandCallback cb) {
  std::move(cb).Run(stored_brand_);
}

void AntServiceImpl::SetScreenH(int32_t v, SetScreenHCallback cb) {
  stored_screen_h_ = v;
  base::AntValues::GetInstance()->screen_h.store(v);
  std::move(cb).Run("ok");
}
void AntServiceImpl::GetScreenH(GetScreenHCallback cb) {
  std::move(cb).Run(stored_screen_h_);
}

void AntServiceImpl::SetScreenW(int32_t v, SetScreenWCallback cb) {
  stored_screen_w_ = v;
  base::AntValues::GetInstance()->screen_w.store(v);
  std::move(cb).Run("ok");
}
void AntServiceImpl::GetScreenW(GetScreenWCallback cb) {
  std::move(cb).Run(stored_screen_w_);
}

}  // namespace content
