<?php

/**
 * @file
 * Management form for the Islandora Simple Workflow module.
 */

/**
 * Builds the management form for the Islandora Simple Workflow module.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 *
 * @return array
 *   An array containing the form to be rendered.
 */
function islandora_simple_workflow_manage_form($form, $form_state) {
  module_load_include('inc', 'islandora_simple_workflow', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/utilities');

  if (isset($form_state['show_confirm'])) {
    $form['confirm_message'] = array(
      '#type' => 'item',
      '#markup' => format_plural(count($form_state['pids']),
      'Are you sure you want to publish the 1 object?',
      'Are you sure you want to publish the @count objects?'),
    );
    $form['confirm_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Confirm'),
      '#weight' => 2,
      '#submit' => array('islandora_simple_workflow_manage_confirm_submit'),
    );
    $form['cancel_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#weight' => 3,
    );
  }
  elseif (isset($form_state['show_purge_confirm'])) {
    $form['confirm_message'] = array(
      '#type' => 'item',
      '#markup' => format_plural(count($form_state['pids']),
      'Are you sure you want to purge the 1 object?',
      'Are you sure you want to purge the @count objects?'),
    );
    $form['confirm_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Confirm'),
      '#weight' => 2,
      '#submit' => array('islandora_simple_workflow_manage_purge_confirm_submit'),
    );
    $form['cancel_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#weight' => 3,
    );
  }
  else {
    $render_parents = variable_get('islandora_simple_workflow_toggle_parent_column', FALSE);
    $inactive_objects = islandora_simple_workflow_get_inactive_objects();

    $rows = array();

    foreach ($inactive_objects as $inactive_object) {
      $object_pid = $inactive_object['object']['value'];
      $rows[$object_pid][] = l($inactive_object['title']['value'] . " (" . $object_pid . ")", "islandora/object/$object_pid");

      if ($render_parents) {
        $object = islandora_object_load($object_pid);

        if ($object) {
          $parents = islandora_get_parents_from_rels_ext($object);
          $inactive_object['parents'] = array();

          foreach ($parents as $parent) {
            $inactive_object['parents'][] = l($parent->id, "islandora/object/$parent->id");
          }

          $rows[$object_pid][] = implode(', ', $inactive_object['parents']);
        }
      }
    }

    $header = array(
      t('Object'),
    );

    if ($render_parents) {
      $header[] = t('Parents');
    }

    $form['management_table'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $rows,
      '#attributes' => array(),
      '#empty' => t('No inactive objects were found.'),
    );
    if (!empty($rows)) {
      $form['submit_selected'] = array(
        '#type' => 'submit',
        '#name' => 'islandora-simple-workflow-submit-selected',
        '#validate' => array('islandora_simple_workflow_manage_publish_selected_validate'),
        '#submit' => array('islandora_simple_workflow_manage_publish_submit'),
        '#value' => t('Publish Selected'),
      );
      $form['submit_all'] = array(
        '#type' => 'submit',
        '#name' => 'islandora-simple-workflow-submit-all',
        '#submit' => array('islandora_simple_workflow_manage_publish_submit'),
        '#value' => t('Publish All'),
      );
      $form['purge_selected'] = array(
        '#type' => 'submit',
        '#name' => 'islandora-simple-workflow-purge-selected',
        '#validate' => array('islandora_simple_workflow_manage_purge_selected_validate'),
        '#submit' => array('islandora_simple_workflow_manage_purge_submit'),
        '#value' => t('Purge Selected'),
      );
      $form['purge_all'] = array(
        '#type' => 'submit',
        '#name' => 'islandora-simple-workflow-purge-all',
        '#submit' => array('islandora_simple_workflow_manage_purge_submit'),
        '#value' => t('Purge All'),
      );
    }
  }
  return $form;
}

/**
 * Validation for the Islandora Simple Workflow management form.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_simple_workflow_manage_publish_selected_validate($form, $form_state) {
  $selected = array_filter($form_state['values']['management_table']);
  if (empty($selected)) {
    form_error($form['management_table'], t('At least one object must be selected to publish!'));
  }
}

/**
 * Submit handler for the publish buttons in the workflow management form.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_simple_workflow_manage_publish_submit(&$form, &$form_state) {
  if ($form_state['triggering_element']['#name'] == 'islandora-simple-workflow-submit-selected') {
    $selected = array_keys(array_filter($form_state['values']['management_table']));
  }
  else {
    $selected = array_keys($form_state['values']['management_table']);
  }
  $form_state['pids'] = $selected;
  // Rebuild to show the confirm form.
  $form_state['rebuild'] = TRUE;
  $form_state['show_confirm'] = TRUE;
}

/**
 * Submit handler for the workflow management confirm form.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_simple_workflow_manage_confirm_submit($form, &$form_state) {
  module_load_include('inc', 'islandora_simple_workflow', 'includes/batch');
  $batch = islandora_simple_workflow_publish_create_batch($form_state['pids']);
  batch_set($batch);
}

/**
 * Validation for the Islandora Simple Workflow management form.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_simple_workflow_manage_purge_selected_validate($form, $form_state) {
  $selected = array_filter($form_state['values']['management_table']);
  if (empty($selected)) {
    form_error($form['management_table'], t('At least one object must be selected to purge!'));
  }
}

/**
 * Submit handler for the purge buttons in the workflow management form.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_simple_workflow_manage_purge_submit($form, &$form_state) {
  if ($form_state['triggering_element']['#name'] == 'islandora-simple-workflow-purge-selected') {
    $selected = array_keys(array_filter($form_state['values']['management_table']));
  }
  else {
    $selected = array_keys($form_state['values']['management_table']);
  }
  $form_state['pids'] = $selected;
  $form['confirm_submit']['#submit'] = array('islandora_simple_workflow_manage_purge_confirm_submit');
  // Rebuild to show the confirm form.
  $form_state['rebuild'] = TRUE;
  $form_state['show_purge_confirm'] = TRUE;
}

/**
 * Submit handler for the workflow management purge confirm form.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_simple_workflow_manage_purge_confirm_submit($form, &$form_state) {
  module_load_include('inc', 'islandora_simple_workflow', 'includes/batch');
  $batch = islandora_simple_workflow_purge_create_batch($form_state['pids']);
  batch_set($batch);
}
