#pragma once

#include "Config.hpp"
#include "config/ManualCalib.hpp"
#include "FormBuilder.hpp"
#include "GUI.hpp"
#include "Localization.hpp"
#include "PreviewPane.hpp"
#include "U8String.hpp"

#include <opencv2/videoio.hpp>
#include <opencv2/videoio/registry.hpp>
#include <wx/frame.h>
#include <wx/msgdlg.h>
#include <wx/notebook.h>
#include <wx/window.h>

#include <functional>
#include <sstream>
#include <string>

/// wxWidgets will delete this
class GUI::MainFrame : protected wxFrame
{
public:
    MainFrame(RefPtr<ITrackerControl> _tracker, const Localization& _lc, UserConfig& _config);

    template <typename F>
    void CallOnMainThread(const F& func);

    void ShowPrompt(const U8String& msg, const std::function<void(bool)>& onClose);
    void ShowPopup(const U8String& msg, PopupStyle style);

    void SetStatus(bool status, StatusItem item);

    /// thread safe.
    void UpdatePreview(const cv::Mat& image, PreviewId id = PreviewId::Main);
    /// thread safe.
    void UpdatePreview(const cv::Mat& image, int constrainSize, PreviewId id = PreviewId::Main);
    /// thread safe.
    bool IsPreviewVisible(PreviewId id = PreviewId::Main);
    void SetPreviewVisible(bool visible = true, PreviewId id = PreviewId::Main, bool userCanDestroy = true);

    /// thread safe.
    /// Get the manual calibration currently shown in the UI.
    cfg::ManualCalib::Real GetManualCalib();
    /// thread safe.
    /// Set the manual calib currently shown in the UI.
    void SetManualCalib(const cfg::ManualCalib::Real& calib);
    /// Set if the manual calib window is visible.
    void SetManualCalibVisible(bool visible = true);

private:
    void OnCloseWindow();
    void OnNotebookPageChanged(wxBookCtrlEvent& evt);

    void SaveParams();
    void ValidateParams();

    void CreateCameraPage(RefPtr<wxNotebook> pages);
    void CreateParamsPage(RefPtr<wxNotebook> pages);
    void CreateLicensePage(RefPtr<wxNotebook> pages);

    U8String CreateCVCaptureAPIToolTip();

    RefPtr<ITrackerControl> tracker;
    const Localization& lc;
    UserConfig& config;

    RefPtr<wxStatusBar> statusBar;
    Form::FormBuilder params;
    Form::FormBuilder cam;

    /// Reference to params sub form
    RefPtr<Form::FormBuilder> manualCalibForm;
    RefPtr<wxCheckBox> manualCalibCheckBox;

    cfg::ManualCalib manualCalib{};

    std::array<PreviewFrame, 2> previews;
};

/// Care should be taken to only call this sparingly from other threads,
/// so not continuously in an update loop for example.
template <typename F>
inline void GUI::MainFrame::CallOnMainThread(const F& func)
{
    CallAfter(func);
}
