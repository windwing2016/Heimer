// This file is part of Heimer.
// Copyright (C) 2018 Jussi Lind <jussi.lind@iki.fi>
//
// Heimer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// Heimer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Heimer. If not, see <http://www.gnu.org/licenses/>.

#include "state_machine.hpp"

#include "application_service.hpp"
#include "service_container.hpp"
#include "simple_logger.hpp"

StateMachine::StateMachine(QObject * parent)
  : QObject(parent)
{
}

void StateMachine::calculateState(StateMachine::Action action)
{
    switch (action) {
    case Action::BackgroundColorChangeRequested:
        m_state = State::ShowBackgroundColorDialog;
        break;

    case Action::EdgeColorChangeRequested:
        m_state = State::ShowEdgeColorDialog;
        break;

    case Action::GridColorChangeRequested:
        m_state = State::ShowGridColorDialog;
        break;

    case Action::ImageAttachmentRequested:
        m_state = State::ShowImageFileDialog;
        break;

    case Action::NodeColorChangeRequested:
        m_state = State::ShowNodeColorDialog;
        break;

    case Action::PngExportSelected:
        m_state = State::ShowPngExportDialog;
        break;

    case Action::TextColorChangeRequested:
        m_state = State::ShowTextColorDialog;
        break;

    case Action::NewSelected:
        m_quitType = QuitType::New;
        if (SIC::instance().applicationService()->isModified()) {
            m_state = State::ShowNotSavedDialog;
        } else {
            m_state = State::InitializeNewMindMap;
        }
        break;

    case Action::MainWindowInitialized:
        m_state = State::InitializeNewMindMap;
        break;

    case Action::NotSavedDialogDiscarded:
    case Action::MindMapSaved:
    case Action::MindMapSavedAs:
        switch (m_quitType) {
        case QuitType::Close:
            m_state = State::Exit;
            break;
        case QuitType::New:
            m_state = State::InitializeNewMindMap;
            break;
        case QuitType::Open:
            m_state = State::ShowOpenDialog;
            break;
        case QuitType::OpenRecent:
            m_state = State::OpenRecent;
            break;
        case QuitType::OpenDrop:
            m_state = State::OpenDrop;
            break;
        default:
            m_state = State::Edit;
            break;
        }
        break;

    case Action::BackgroundColorChanged:
    case Action::EdgeColorChanged:
    case Action::GridColorChanged:
    case Action::ImageLoadFailed:
    case Action::LayoutOptimized:
    case Action::MindMapOpened:
    case Action::MindMapSaveFailed:
    case Action::MindMapSaveAsCanceled:
    case Action::MindMapSaveAsFailed:
    case Action::NewMindMapInitialized:
    case Action::NodeColorChanged:
    case Action::NotSavedDialogCanceled:
    case Action::OpeningMindMapCanceled:
    case Action::OpeningMindMapFailed:
    case Action::PngExported:
    case Action::SvgExported:
    case Action::TextColorChanged:
        m_quitType = QuitType::None;
        m_state = State::Edit;
        break;

    case Action::OpenSelected:
        m_quitType = QuitType::Open;
        if (SIC::instance().applicationService()->isModified()) {
            m_state = State::ShowNotSavedDialog;
        } else {
            m_state = State::ShowOpenDialog;
        }
        break;

    case Action::QuitSelected:
        m_quitType = QuitType::Close;
        if (SIC::instance().applicationService()->isModified()) {
            m_state = State::ShowNotSavedDialog;
        } else {
            m_state = State::Exit;
        }
        break;

    case Action::NotSavedDialogAccepted:
    case Action::SaveSelected:
        if (SIC::instance().applicationService()->canBeSaved()) {
            m_state = State::Save;
        } else {
            m_state = State::ShowSaveAsDialog;
        }
        break;

    case Action::SaveAsSelected:
        m_state = State::ShowSaveAsDialog;
        break;

    case Action::SvgExportSelected:
        m_state = State::ShowSvgExportDialog;
        break;

    case Action::RecentFileSelected:
        m_quitType = QuitType::OpenRecent;
        if (SIC::instance().applicationService()->isModified()) {
            m_state = State::ShowNotSavedDialog;
        } else {
            m_state = State::OpenRecent;
        }
        break;

    case Action::DropFileSelected:
        m_quitType = QuitType::OpenDrop;
        if (SIC::instance().applicationService()->isModified()) {
            m_state = State::ShowNotSavedDialog;
        } else {
            m_state = State::OpenDrop;
        }
        break;

    case Action::LayoutOptimizationRequested:
        m_state = State::ShowLayoutOptimizationDialog;
        break;

    default:
        juzzlin::L().warning() << "Action " << static_cast<int>(action) << " not handled!";
    }

    emit stateChanged(m_state);
}
