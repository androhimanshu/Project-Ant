#ifndef CONTENT_BROWSER_ANT_ANT_SERVICE_IMPL_H_
#define CONTENT_BROWSER_ANT_ANT_SERVICE_IMPL_H_

#include <string>

#include "content/common/content_export.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver_set.h"
#include "mojo/public/cpp/bindings/remote_set.h"
#include "third_party/blink/public/mojom/ant/ant_service.mojom.h"

namespace content {

class CONTENT_EXPORT AntServiceImpl : public blink::mojom::AntService {
 public:
  AntServiceImpl();
  ~AntServiceImpl() override;

  AntServiceImpl(const AntServiceImpl&) = delete;
  AntServiceImpl& operator=(const AntServiceImpl&) = delete;

  static AntServiceImpl* GetInstance();

  void BindReceiver(
      mojo::PendingReceiver<blink::mojom::AntService> receiver);

  void SetValue(int32_t value, SetValueCallback cb) override;
  void GetValue(GetValueCallback cb) override;
  void AddObserver(
      mojo::PendingRemote<blink::mojom::AntValueObserver> observer) override;
  void SetString(const std::string& text, SetStringCallback cb) override;
  void GetString(GetStringCallback cb) override;
  void SetGpu(const std::string& v, SetGpuCallback cb) override;
  void GetGpu(GetGpuCallback cb) override;
  void SetModel(const std::string& v, SetModelCallback cb) override;
  void GetModel(GetModelCallback cb) override;
  void SetAversion(const std::string& v, SetAversionCallback cb) override;
  void GetAversion(GetAversionCallback cb) override;
  void SetBversion(const std::string& v, SetBversionCallback cb) override;
  void GetBversion(GetBversionCallback cb) override;
  void SetVendor(const std::string& v, SetVendorCallback cb) override;
  void GetVendor(GetVendorCallback cb) override;
  void SetCpu(int32_t value, SetCpuCallback cb) override;
  void GetCpu(GetCpuCallback cb) override;
  void SetBrand(const std::string& v, SetBrandCallback cb) override;
  void GetBrand(GetBrandCallback cb) override;
  void SetScreenH(int32_t value, SetScreenHCallback cb) override;
  void GetScreenH(GetScreenHCallback cb) override;
  void SetScreenW(int32_t value, SetScreenWCallback cb) override;
  void GetScreenW(GetScreenWCallback cb) override;

 private:
  int32_t     stored_value_    = 0;
  std::string stored_string_;
  std::string stored_gpu_;
  std::string stored_model_;
  std::string stored_aversion_;
  std::string stored_bversion_;
  std::string stored_vendor_;
  int32_t     stored_cpu_      = 0;
  std::string stored_brand_;
  int32_t     stored_screen_h_ = 0;
  int32_t     stored_screen_w_ = 0;

  mojo::ReceiverSet<blink::mojom::AntService>     receivers_;
  mojo::RemoteSet<blink::mojom::AntValueObserver> observers_;
};

}  // namespace content

#endif  // CONTENT_BROWSER_ANT_ANT_SERVICE_IMPL_H_
