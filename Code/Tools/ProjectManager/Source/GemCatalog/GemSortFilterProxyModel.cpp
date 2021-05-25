/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#include <GemCatalog/GemSortFilterProxyModel.h>
#include <QItemSelectionModel>

namespace O3DE::ProjectManager
{
    GemSortFilterProxyModel::GemSortFilterProxyModel(GemModel* sourceModel, QObject* parent)
        : QSortFilterProxyModel(parent)
        , m_sourceModel(sourceModel)
    {
        setSourceModel(sourceModel);
        m_selectionProxyModel = new AzQtComponents::SelectionProxyModel(sourceModel->GetSelectionModel(), this, parent);
    }

    bool GemSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
    {
        // Do not use sourceParent->child because an invalid parent does not produce valid children (which our index function does)
        QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);
        if (!sourceIndex.isValid())
        {
            return false;
        }

        if (!m_sourceModel->GetName(sourceIndex).contains(m_searchString, Qt::CaseInsensitive))
        {
            return false;
        }

        // Gem origins
        if (m_gemOriginFilter)
        {
            bool supportsAnyFilteredGemOrigin = false;
            for (int i = 0; i < GemInfo::NumGemOrigins; ++i)
            {
                const GemInfo::GemOrigin filteredGemOrigin = static_cast<GemInfo::GemOrigin>(1 << i);
                if (m_gemOriginFilter & filteredGemOrigin)
                {
                    if ((GemModel::GetGemOrigin(sourceIndex) == filteredGemOrigin))
                    {
                        supportsAnyFilteredGemOrigin = true;
                        break;
                    }
                }
            }
            if (!supportsAnyFilteredGemOrigin)
            {
                return false;
            }
        }

        // Platform
        if (m_platformFilter)
        {
            bool supportsAnyFilteredPlatform = false;
            for (int i = 0; i < GemInfo::NumPlatforms; ++i)
            {
                const GemInfo::Platform filteredPlatform = static_cast<GemInfo::Platform>(1 << i);
                if (m_platformFilter & filteredPlatform)
                {
                    if ((GemModel::GetPlatforms(sourceIndex) & filteredPlatform))
                    {
                        supportsAnyFilteredPlatform = true;
                        break;
                    }
                }
            }
            if (!supportsAnyFilteredPlatform)
            {
                return false;
            }
        }

        // Types (Asset, Code, Tool)
        if (m_typeFilter)
        {
            bool supportsAnyFilteredType = false;
            for (int i = 0; i < GemInfo::NumTypes; ++i)
            {
                const GemInfo::Type filteredType = static_cast<GemInfo::Type>(1 << i);
                if (m_typeFilter & filteredType)
                {
                    if ((GemModel::GetTypes(sourceIndex) & filteredType))
                    {
                        supportsAnyFilteredType = true;
                        break;
                    }
                }
            }
            if (!supportsAnyFilteredType)
            {
                return false;
            }
        }

        // Features
        if (!m_featureFilter.isEmpty())
        {
            bool containsFilterFeature = false;
            const QStringList features = m_sourceModel->GetFeatures(sourceIndex);
            for (const QString& feature : features)
            {
                if (m_featureFilter.contains(feature))
                {
                    containsFilterFeature = true;
                    break;
                }
            }
            if (!containsFilterFeature)
            {
                return false;
            }
        }

        return true;
    }

    void GemSortFilterProxyModel::InvalidateFilter()
    {
        invalidate();
        emit OnInvalidated();
    }
} // namespace O3DE::ProjectManager