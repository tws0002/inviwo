/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2018 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#include <modules/webbrowser/properties/propertycefsynchronizer.h>
#include <modules/webbrowser/properties/boolpropertywidgetcef.h>
#include <modules/webbrowser/properties/buttonpropertywidgetcef.h>
#include <modules/webbrowser/properties/ordinalpropertywidgetcef.h>
#include <modules/webbrowser/properties/stringpropertywidgetcef.h>

namespace inviwo {
PropertyCefSynchronizer::PropertyCefSynchronizer() {
    registerPropertyWidget<BoolPropertyWidgetCEF, BoolProperty>(PropertySemantics("Default"));
    registerPropertyWidget<ButtonPropertyWidgetCEF, ButtonProperty>(PropertySemantics("Default"));

    registerPropertyWidget<FloatPropertyWidgetCEF, FloatProperty>(PropertySemantics("Default"));
    registerPropertyWidget<DoublePropertyWidgetCEF, DoubleProperty>(PropertySemantics("Default"));
    registerPropertyWidget<IntPropertyWidgetCEF, IntProperty>(PropertySemantics("Default"));
    registerPropertyWidget<IntSizeTPropertyWidgetCEF, IntSizeTProperty>(
        PropertySemantics("Default"));
    registerPropertyWidget<Int64PropertyWidgetCEF, Int64Property>(PropertySemantics("Default"));

    registerPropertyWidget<StringPropertyWidgetCEF, StringProperty>(PropertySemantics("Default"));
};

void PropertyCefSynchronizer::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                        int httpStatusCode) {
    // synchronize all properties
    // Ok to send javascript commands when fame loaded
    for (auto& widget : widgets_) {
        widget->setFrame(frame);
        widget->updateFromProperty();
    }
}

bool PropertyCefSynchronizer::OnQuery(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                      int64 query_id, const CefString& request, bool persistent,
                                      CefRefPtr<Callback> callback) {

    const std::string& message = request;
    auto widget =
        std::find_if(std::begin(widgets_), std::end(widgets_), [message](const auto& widget) {
            return message.find(widget->getHtmlId()) != std::string::npos;
        });
    if (widget != widgets_.end()) {
        return (*widget)->onQuery(browser, frame, query_id, request, persistent, callback);
    }

    return false;
}

void PropertyCefSynchronizer::startSynchronize(Property* property) {
    // We cannot use path since Processor is not set until after construction.
    // auto path = property->getPath();
    // auto htmlId = std::accumulate(std::next(path.begin()), path.end(), path[0],
    //                                     [](std::string &s, const std::string &piece) ->
    //                                     decltype(auto) { return s += "." + piece; });
    auto htmlId = property->getIdentifier();
    startSynchronize(property, htmlId);
}

void PropertyCefSynchronizer::startSynchronize(Property* property, std::string htmlId) {
    auto widget = dynamic_cast<PropertyWidgetCEF*>(htmlWidgetFactory_.create(property).release());
    if (!widget) {
        throw Exception("No HTML property widget for " + property->getClassIdentifier());
    }
    widget->setHtmlId(htmlId);
    // auto widget = std::make_unique<OrdinalPropertyWidgetCEF<T>>(property,
    // browser_->GetMainFrame(), htmlId);
    widgets_.emplace_back(std::move(widget));
}

void PropertyCefSynchronizer::stopSynchronize(Property* property) {
    util::erase_remove_if(widgets_,
                          [property](auto& widget) { return property == widget->getProperty(); });
}

}  // namespace inviwo