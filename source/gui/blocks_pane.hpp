﻿#ifndef _HS_3D_RECONSTRUCTOR_BLOCKS_PANE_HPP_
#define _HS_3D_RECONSTRUCTOR_BLOCKS_PANE_HPP_

#include <memory>
#include <bitset>
#include <queue>

#include <QAction>
#include <QIcon>
#include <QProgressBar>
#include <QBrush>

#include "database/database_mediator.hpp"
#include "workflow/common/workflow_step.hpp"
#include "workflow/feature_match/feature_match_config.hpp"
#include "workflow/photo_orientation/incremental_photo_orientation.hpp"
#include "workflow/point_cloud/pmvs_point_cloud.hpp"
#include "gui/manager_pane.hpp"
#include "gui/blocks_tree_widget.hpp"
#include "gui/photo_orientation_info_widget.hpp"
//#include "workflow/mesh_surface/poisson_surface_model.hpp"
#include "workflow/mesh_surface/surface_model_config.hpp"
#include "workflow/texture/rough_texture.hpp"

namespace hs
{
namespace recon
{
namespace gui
{

class BlocksPane : public ManagerPane, public hs::recon::db::DatabaseObserver
{
  Q_OBJECT
public:
  enum ItemSelectedFlag
  {
    BLOCK_SELECTED = 0,
    PHOTOS_SELECTED,
    PHOTO_SELECTED,
    FEATURE_MATCH_SELECTED,
    PHOTO_ORIENTATION_SELECTED,
    POINT_CLOUD_SELECTED,
    SURFACE_MODEL_SELECTED,
    TEXTURE_SELECTED,
    NUMBER_OF_ITEM_SELECTED_FLAGS
  };
  typedef std::bitset<NUMBER_OF_ITEM_SELECTED_FLAGS> ItemSelectedMask;
  struct WorkflowStepEntry
  {
    uint id;
    hs::recon::workflow::WorkflowStepConfigPtr config;
  };
  struct WorkflowConfig
  {
    std::string workflow_intermediate_directory;
    std::queue<WorkflowStepEntry> step_queue;
  };
  typedef std::shared_ptr<hs::recon::workflow::WorkflowStep> WorkflowStepPtr;
public:
  BlocksPane(QWidget* parent = 0);

  virtual void Response(int request_flag, void* response);

private slots:
  void OnTimeout();
  void OnItemDoubleClicked(QTreeWidgetItem* item, int column);

  void OnActionAddBlockTriggered();
  void OnActionAddWorkflowTriggered();
  void OnActionCopyTriggered();
  void OnActionRemoveTriggered();
  void OnActionInfoTriggered();

  void OnBlockItemSelected(uint block_id);
  void OnPhotosInOneBlockSelected(uint block_id,
                                  const std::vector<uint>& photo_ids);
  void OnPhotosItemSelected(uint block_id);
  void OnFeatureMatchItemSelected(uint feature_match_id);
  void OnPhotoOrientationItemSelected(uint photo_orientation_id);
  void OnPointCloudItemSelected(uint point_cloud_id);
  void OnSurfaceModelItemSelected(uint surface_model_id);
  void OnTextureItemSelected(uint texture_id);

private:
  void ActivatePhotoOrientationItem(QTreeWidgetItem* photo_orientation_item);
  void ActivatePointCloudItem(QTreeWidgetItem* point_cloud_item);
  void ActivateSurfaceModelItem(QTreeWidgetItem* surface_model_item);

  int SetWorkflowStep(const std::string& workflow_intermediate_directory,
                      WorkflowStepEntry& workflow_step_entry);

  int AddFeatureMatchStep(uint block_id,
                          workflow::FeatureMatchConfigPtr feature_match_config,
                          WorkflowConfig& workflow_config);

  int AddPhotoOrientationStep(
    uint feature_match_id,
    workflow::PhotoOrientationConfigPtr photo_orientation_config,
    WorkflowConfig& workflow_config);

  int AddPointCloudStep(
    uint photo_orientation_id,
    workflow::PointCloudConfigPtr point_cloud_config,
    WorkflowConfig& workflow_config);

  int AddSurfaceModelStep(
    uint point_cloud_id,
    workflow::MeshSurfaceConfigPtr surface_model_config,
    WorkflowConfig& workflow_config);

  int AddTextureStep(
    uint surface_model_id,
    workflow::TextureConfigPtr texture_config,
    WorkflowConfig& workflow_config);

  WorkflowStepPtr SetFeatureMatchStep(
    const std::string& workflow_intermediate_directory,
    WorkflowStepEntry& workflow_step_entry);

  WorkflowStepPtr SetPhotoOrientationStep(
    const std::string& workflow_intermediate_directory,
    WorkflowStepEntry& workflow_step_entry);

  WorkflowStepPtr SetPointCloudStep(
    const std::string& workflow_intermediate_directory,
    WorkflowStepEntry& workflow_step_entry);

  WorkflowStepPtr SetSurfaceModelStep(
    const std::string& workflow_intermediate_directory,
    WorkflowStepEntry& workflow_step_entry);

  WorkflowStepPtr SetTextureStep(
    const std::string& workflow_intermediate_directory,
    WorkflowStepEntry& workflow_step_entry);

signals:
  void PhotoOrientationActivated(uint photo_orientation_id);
  void PointCloudActivated(uint point_cloud_id);
  void SurfaceModelActivated(uint surface_model_id);

private:
  BlocksTreeWidget* blocks_tree_widget_;
  PhotoOrientationInfoWidget* photo_orientation_info_widget_;

  QBrush backup_background_;

  QIcon icon_add_block_;
  QIcon icon_copy_;
  QIcon icon_remove_;
  QIcon icon_add_workflow_;
  QIcon icon_info_;

  QAction* action_add_block_;
  QAction* action_copy_;
  QAction* action_remove_;
  QAction* action_add_workflow_;
  QAction* action_info_;

  uint selected_block_id_;
  std::vector<uint> photo_ids_;
  uint selected_feature_match_id_;
  uint selected_photo_orientation_id_;
  uint selected_point_cloud_id_;
  uint selected_surface_model_id_;
  uint selected_texture_id_;

  uint activated_photo_orientation_id_;
  uint activated_point_cloud_id_;
  uint activated_surface_model_id_;

  QTimer* timer_;
  QProgressBar* progress_bar_;

  WorkflowStepPtr current_step_;
  std::queue<WorkflowConfig> workflow_queue_;

  ItemSelectedMask item_selected_mask_;
};

}
}
}

#endif
