<?php

/**
 * @file
 * The single url processor class.
 */

/**
 * Url processor plugin that ensures facets link only to themselves
 *
 * This plugin retrieves facet data from $_GET, and stored all information in
 * the "f" query string variable by default.
 */
class FacetapiUrlProcessorSingle extends FacetapiUrlProcessorStandard {

  /**
   * Implements FacetapiUrlProcessor::getQueryString().
   */
  public function getQueryString(array $facet, array $values, $active) {
    $qstring = $this->params;
    $active_items = $this->adapter->getActiveItems($facet);

    // Appends to qstring if inactive, removes if active.
    foreach ($values as $value) {
      if ($active && isset($active_items[$value])) {
        unset($qstring[$this->filterKey][$active_items[$value]['pos']]);
      }
      elseif (!$active) {
        $field_alias = rawurlencode($facet['field alias']);
        $qstring[$this->filterKey][] = $field_alias . ':' . $value;
      }
    }

    // Removes duplicates, resets array keys and returns query string.
    // @see http://drupal.org/node/1340528
    $qstring[$this->filterKey] = array_values(array_unique($qstring[$this->filterKey]));

    // remove active filter from query string
    if ($this->isSingleFacet($facet) && count($qstring[$this->filterKey]) > 1) {
      array_shift($qstring[$this->filterKey]);
    }

    return array_filter($qstring);
  }

  /**
   * Determine if this facet is supposed to be single
   */
  public function isSingleFacet(array $facet) {
    $facet_settings = $this->adapter->getFacetSettingsGlobal($facet);
    return isset($facet_settings->settings['single_facet']) ? $facet_settings->settings['single_facet'] : false;
  }
}
